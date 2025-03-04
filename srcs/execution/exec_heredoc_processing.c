/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:31:42 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 21:36:36 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int	wait_heredoc_child(pid_t pid, int pipe_fd[2], t_exec *exec)
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

int	setup_terminal_and_delimiter(struct termios *orig_term,
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

int	process_heredoc_line(int fd, char *clean_delim, t_tools *tools,
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

int	process_heredoc(int fd, char *delimiter, t_tools *tools)
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
