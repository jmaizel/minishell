/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 17:15:11 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	setup_child_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_heredoc_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_parent_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	is_quoted_delimiter(char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\''))
			return (1);
	}
	return (0);
}

char	*remove_quotes(char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\''))
			return (ft_substr(delimiter, 1, len - 2));
	}
	return (ft_strdup(delimiter));
}

char	*expand_heredoc_line(char *line, t_tools *tools)
{
	char	*expanded;
	char	*exit_str;

	if (!ft_strchr(line, '$'))
		return (ft_strdup(line));
	if (ft_strcmp(line, "$?") == 0)
	{
		exit_str = ft_itoa(tools->exit_code);
		return (exit_str);
	}
	expanded = expand_str(line, tools);
	if (!expanded)
		return (ft_strdup(line));
	return (expanded);
}

static void	write_expanded_line(int fd, char *line, t_tools *tools, int quoted)
{
	char	*expanded;

	if (!quoted && ft_strchr(line, '$'))
	{
		expanded = expand_heredoc_line(line, tools);
		if (expanded)
		{
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, line, ft_strlen(line));
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static int	setup_terminal_and_delimiter(struct termios *orig_term,
		char **clean_delim, char *delimiter, int *quoted)
{
	tcgetattr(STDIN_FILENO, orig_term);
	orig_term->c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, orig_term);
	*quoted = is_quoted_delimiter(delimiter);
	*clean_delim = remove_quotes(delimiter);
	if (!*clean_delim)
		return (tcsetattr(STDIN_FILENO, TCSANOW, orig_term), 1);
	return (0);
}

static int	process_heredoc_line(int fd, char *clean_delim, t_tools *tools,
		int quoted)
{
	char	*line;

	line = readline("heredoc> ");
	if (!line)
		return (1);
	if (ft_strcmp(line, clean_delim) == 0)
	{
		free(line);
		return (2);
	}
	write_expanded_line(fd, line, tools, quoted);
	free(line);
	return (0);
}

static int	process_heredoc(int fd, char *delimiter, t_tools *tools)
{
	char			*clean_delim;
	struct termios	orig_term;
	int				quoted;
	int				result;
	int				signal_status;

	if (setup_terminal_and_delimiter(&orig_term, &clean_delim, delimiter,
			&quoted))
		return (1);
	while (!g_signal_received)
	{
		result = process_heredoc_line(fd, clean_delim, tools, quoted);
		if (result == 1)
		{
			free(clean_delim);
			tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
			return (1);
		}
		if (result == 2)
			break ;
	}
	signal_status = g_signal_received;
	free(clean_delim);
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
	return (signal_status);
}

static void	process_intermediate_heredocs(t_parsed_cmd *cmd, t_exec *exec,
		int pipe_fd[2])
{
	int	i;
	int	temp_fd;

	if (cmd->heredoc_count <= 1)
		return ;
	i = 0;
	while (i < cmd->heredoc_count - 1 && !g_signal_received)
	{
		temp_fd = open("/dev/null", O_WRONLY);
		if (temp_fd == -1)
			exit(1);
		if (process_heredoc(temp_fd, cmd->heredoc_delim[i], exec->tools))
		{
			close(temp_fd);
			close(pipe_fd[1]);
			exit(1);
		}
		close(temp_fd);
		i++;
	}
}

static void	child_heredoc_process(t_parsed_cmd *cmd, t_exec *exec,
		int pipe_fd[2])
{
	setup_child_heredoc_signals();
	close(pipe_fd[0]);
	process_intermediate_heredocs(cmd, exec, pipe_fd);
	if (!g_signal_received)
	{
		if (process_heredoc(pipe_fd[1], cmd->heredoc_delim[cmd->heredoc_count
				- 1], exec->tools))
		{
			close(pipe_fd[1]);
			exit(1);
		}
	}
	close(pipe_fd[1]);
	exit(0);
}

static int	wait_heredoc_child(pid_t pid, int pipe_fd[2], t_exec *exec)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			exec->exit_status = 130;
			write(STDERR_FILENO, "\n", 1);
		}
		return (-1);
	}
	return (pipe_fd[0]);
}

int	handle_heredoc(t_parsed_cmd *cmd, t_exec *exec)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	g_signal_received = 0;
	setup_parent_heredoc_signals();
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
		child_heredoc_process(cmd, exec, pipe_fd);
	return (wait_heredoc_child(pid, pipe_fd, exec));
}
