/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 12:35:15 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_child_signal(int sig)
{
	(void)sig;
	g_signal_received = 1;
}

static void	handle_parent_signal(int sig)
{
	(void)sig;
}

static void	setup_child_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_child_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	setup_parent_heredoc_signals(void)
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

static int	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (-1);
	while (!g_signal_received)
	{
		line = readline("heredoc> ");
		if (!line || g_signal_received)
		{
			free(buffer);
			free(line);
			return (-1);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			write(fd, buffer, ft_strlen(buffer));
			free(line);
			free(buffer);
			return (0);
		}
		buffer = append_to_buffer(buffer, line);
		free(line);
		if (!buffer)
			return (-1);
	}
	free(buffer);
	return (-1);
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
		status = write_heredoc_content(pipe_fd[1], delimiter);
		close(pipe_fd[1]);
		if (g_signal_received)
			ft_putchar_fd('\n', STDERR_FILENO);
		exit((status == -1) ? 1 : 0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	g_signal_received = 0;
	setup_interactive_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		g_signal_received = 1;
		return (-1);
	}
	if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (0);
}
