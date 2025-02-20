/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/20 21:23:31 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/20 21:18:38 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_child_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(1, "\n", 1);
		close(STDIN_FILENO);
	}
}

static void	handle_parent_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(1, "\n", 1);
	}
}

void	setup_child_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_child_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_parent_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_parent_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static char	*append_to_buffer(char *buffer, const char *line)
{
	char	*temp;
	char	*new_buffer;

	temp = ft_strjoin(buffer, line);
	if (!temp)
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_strjoin(temp, "\n");
	free(temp);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	free(buffer);
	return (new_buffer);
}

static char	*clean_quotes_from_delimiter(char *delim)
{
	size_t	len;
	char	*cleaned;

	if (!delim)
		return (NULL);
	len = ft_strlen(delim);
	if (len >= 2)
	{
		if ((delim[0] == '"' && delim[len - 1] == '"') 
			|| (delim[0] == '\'' && delim[len - 1] == '\''))
		{
			cleaned = ft_substr(delim, 1, len - 2);
			return (cleaned);
		}
	}
	return (ft_strdup(delim));
}

static int	write_heredoc_multiple(int fd, char **delims, int count)
{
	char	*line;
	char	*buffer;
	int		saved_stdout;
	char	*cleaned_delim;

	buffer = ft_strdup("");
	cleaned_delim = clean_quotes_from_delimiter(delims[count - 1]);
	if (!buffer || !cleaned_delim)
	{
		free(buffer);
		free(cleaned_delim);
		return (-1);
	}
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		free(buffer);
		free(cleaned_delim);
		return (-1);
	}
	g_signal_received = 0;
	while (!g_signal_received)
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		line = readline("heredoc> ");
		dup2(saved_stdout, STDOUT_FILENO);
		if (!line || g_signal_received)
		{
			free(buffer);
			free(cleaned_delim);
			if (line)
				free(line);
			close(saved_stdout);
			return (-1);
		}
		if (ft_strcmp(line, cleaned_delim) == 0)
		{
			write(fd, buffer, ft_strlen(buffer));
			free(line);
			free(buffer);
			free(cleaned_delim);
			close(saved_stdout);
			return (0);
		}
		buffer = append_to_buffer(buffer, line);
		free(line);
		if (!buffer)
		{
			free(cleaned_delim);
			close(saved_stdout);
			return (-1);
		}
	}
	free(buffer);
	free(cleaned_delim);
	close(saved_stdout);
	return (-1);
}

int	handle_heredoc_multiple(t_parsed_cmd *cmd)
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
		close(pipe_fd[0]);
		if (write_heredoc_multiple(pipe_fd[1], cmd->heredoc_delim,
				cmd->heredoc_count) == -1)
		{
			close(pipe_fd[1]);
			exit(1);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}

int	handle_heredoc(char *delimiter)
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
		close(pipe_fd[0]);
		if (write_heredoc_multiple(pipe_fd[1], &delimiter, 1) == -1)
		{
			close(pipe_fd[1]);
			exit(1);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
