/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:21:48 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

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

static int	handle_heredoc_readline(int fd, char *delim, t_tools *tools, int q)
{
	char	*line;
	char	*expanded;

	while (!g_signal_received)
	{
		line = readline("heredoc> ");
		if (!line)
			return (1);
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (!q && ft_strchr(line, '$'))
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
		free(line);
	}
	return (g_signal_received);
}

static int	process_heredoc(int fd, char *delimiter, t_tools *tools)
{
	char			*clean_delim;
	struct termios	orig_term;
	int				quoted;
	int				result;

	tcgetattr(STDIN_FILENO, &orig_term);
	orig_term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
	quoted = is_quoted_delimiter(delimiter);
	clean_delim = remove_quotes(delimiter);
	if (!clean_delim)
		return (tcsetattr(STDIN_FILENO, TCSANOW, &orig_term), 1);
	result = handle_heredoc_readline(fd, clean_delim, tools, quoted);
	free(clean_delim);
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
	return (result);
}

static void	process_intermediate_heredocs(t_parsed_cmd *cmd, t_exec *exec,
		int *pipe_fd)
{
	int	i;
	int	temp_fd;

	close(pipe_fd[0]);
	if (cmd->heredoc_count > 1)
	{
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
}

int	handle_heredoc(t_parsed_cmd *cmd, t_exec *exec)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

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
	{
		setup_child_heredoc_signals();
		process_intermediate_heredocs(cmd, exec, pipe_fd);
		if (!g_signal_received && process_heredoc(pipe_fd[1],
				cmd->heredoc_delim[cmd->heredoc_count - 1], exec->tools))
			exit(1);
		close(pipe_fd[1]);
		exit(0);
	}
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
