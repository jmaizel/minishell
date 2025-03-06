/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:02 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/06 13:45:06 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int	count_pipes(t_pip *pipeline)
{
	int		count;
	t_pip	*current;

	count = 0;
	current = pipeline;
	while (current && current->next)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	close_pipe(int pipe_fd[2])
{
	if (pipe_fd[PIPE_READ] != -1)
		close(pipe_fd[PIPE_READ]);
	if (pipe_fd[PIPE_WRITE] != -1)
		close(pipe_fd[PIPE_WRITE]);
}

void	wait_all_processes(t_exec *exec, int process_count)
{
	int		i;
	int		status;
	pid_t	pid;

	i = 0;
	while (i < process_count)
	{
		pid = wait(&status);
		if (pid > 0)
		{
			if (WIFEXITED(status))
				exec->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					exec->exit_status = 130;
				else if (WTERMSIG(status) == SIGQUIT)
					exec->exit_status = 131;
				else
					exec->exit_status = 128 + WTERMSIG(status);
			}
			i++;
		}
	}
	restore_signals();
}
