/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/08 17:45:13 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static int	read_heredoc(int pipe_fd[2], char *delimiter)
{
	char	*line;

	close(pipe_fd[0]);
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipe_fd[1]);
			exit(0);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (-1);

	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}

	if (pid == 0)
		read_heredoc(pipe_fd, delimiter);

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);

	if (WIFSIGNALED(status))
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
