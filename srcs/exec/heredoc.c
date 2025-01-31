/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:00:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 15:23:08 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
	exit(130);
}

static void	handle_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

static char	*create_heredoc_filename(void)
{
	static int	counter = 0;
	char		*pid_str;
	char		*count_str;
	char		*temp;
	char		*filename;

	pid_str = ft_itoa(getpid());
	count_str = ft_itoa(counter++);
	if (!pid_str || !count_str)
	{
		free(pid_str);
		free(count_str);
		return (NULL);
	}
	temp = ft_strjoin("/tmp/.heredoc_", pid_str);
	free(pid_str);
	if (!temp)
	{
		free(count_str);
		return (NULL);
	}
	filename = ft_strjoin(temp, count_str);
	free(temp);
	free(count_str);
	return (filename);
}

static int	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (0);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (1);
}

void	handle_heredoc(char *delim, t_pip *pip)
{
	char	*filename;
	int		fd;
	pid_t	pid;
	int		status;

	filename = create_heredoc_filename();
	if (!filename)
		handle_error("heredoc: filename creation failed");
	
	pid = fork();
	if (pid == 0)
	{
		handle_heredoc_signals();
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
		if (fd == -1)
			handle_error("heredoc: open failed");
		if (write_heredoc_content(fd, delim))
			exit(130);
		close(fd);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		unlink(filename);
		free(filename);
		exit(130);
	}
	
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		handle_error("heredoc: open failed");
	
	if (pip && pip->redirection)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			handle_error("heredoc: dup2 failed");
	}
	close(fd);
	unlink(filename);
	free(filename);
}
