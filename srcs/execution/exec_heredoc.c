/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:45:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 20:38:56 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = SIG_DFL;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

static void	restore_heredoc_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = SIG_IGN;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

static char	*handle_heredoc_line(char *buffer, char *line)
{
	char	*new_buffer;
	size_t	total_len;

	if (!line)
		return (NULL);
	total_len = ft_strlen(buffer) + ft_strlen(line) + 2;
	if (total_len > HEREDOC_MAX_SIZE)
	{
		ft_putstr_fd("minishell: heredoc: maximum size exceeded\n", 2);
		free(buffer);
		free(line);
		return (NULL);
	}
	new_buffer = ft_strjoin(buffer, line);
	if (!new_buffer)
	{
		free(buffer);
		free(line);
		return (NULL);
	}
	free(buffer);
	free(line);
	buffer = ft_strjoin(new_buffer, "\n");
	free(new_buffer);
	return (buffer);
}

static int	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (-1);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			free(buffer);
			return (0);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			write(fd, buffer, ft_strlen(buffer));
			free(buffer);
			return (0);
		}
		buffer = handle_heredoc_line(buffer, line);
		if (!buffer)
			return (-1);
	}
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (-1);
	restore_heredoc_signals();
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		setup_heredoc_signals();
		close(pipe_fd[0]);
		status = write_heredoc_content(pipe_fd[1], delimiter);
		close(pipe_fd[1]);
		exit(status == -1);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		g_signal_received = 1;
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}
