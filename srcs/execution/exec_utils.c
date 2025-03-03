/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:02 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:37:05 by jmaizel          ###   ########.fr       */
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
	int	i;
	int	status;

	i = 0;
	while (i < process_count)
	{
		if (wait(&status) == exec->process.pid)
		{
			if (WIFEXITED(status))
				exec->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exec->exit_status = WTERMSIG(status) + 128;
		}
		i++;
	}
}
