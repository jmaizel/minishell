/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:14:56 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 21:17:54 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	close_all_pipes(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipes[i][0] != -1)
			close(pipes[i][0]);
		if (pipes[i][1] != -1)
			close(pipes[i][1]);
		i++;
	}
}

int	init_pids_array(t_exec *exec, pid_t **pids)
{
	*pids = malloc(sizeof(pid_t) * (exec->pipe_count + 1));
	if (!*pids)
		return (0);
	return (1);
}

int	clean_fork_resources(pid_t *pids, int pipes[][2], int count)
{
	free(pids);
	close_all_pipes(pipes, count);
	return (1);
}

int	cleanup_and_return(int pipes[][2], t_exec *exec, pid_t *pids)
{
	close_all_pipes(pipes, exec->pipe_count);
	wait_all_processes(exec, exec->pipe_count + 1);
	free(pids);
	return (exec->exit_status);
}

int	fork_and_execute_pid(t_fork_params *params)
{
	ft_memcpy(params->pipes, params->pipes, sizeof(int) * 1024 * 2);
	params->pids[params->i] = fork_and_execute((t_exec_params *)params);
	if (params->pids[params->i] == -1)
		return (1);
	return (0);
}
