
#include "../../includes/execution.h"

static void	handle_heredoc_child(int *pipe_fd, char *delimiter)
{
	char	*line;

	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
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
		handle_heredoc_child(pipe_fd, delimiter);

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);

	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		return (-1);
	}

	// Rediriger l'entrée standard vers le pipe de lecture
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);

	return (0);
}