/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:06:14 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/06 13:45:22 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	setup_pipes(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipe(pipes[i]) == -1)
		{
			close_all_pipes(pipes, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	setup_fork_params(t_fork_params *params, t_pip *current,
		t_exec *exec, int i)
{
	params->cmd = current;
	params->exec = exec;
	params->i = i;
}

static void	copy_pipes_to_params(t_fork_params *params, int pipes[][2])
{
	ft_memcpy(params->pipes, pipes, sizeof(int) * 1024 * 2);
}

static int	exec_pipeline_pids(t_pip *pipeline, t_exec *exec, int pipes[][2],
		int heredoc_fd)
{
	t_pip			*current;
	pid_t			*pids;
	int				i;
	t_fork_params	params;

	if (!init_pids_array(exec, &pids))
	{
		close_all_pipes(pipes, exec->pipe_count);
		return (1);
	}
	i = 0;
	current = pipeline;
	while (current)
	{
		setup_fork_params(&params, current, exec, i);
		copy_pipes_to_params(&params, pipes);
		params.heredoc_fd = heredoc_fd;
		params.pids = pids;
		if (fork_and_execute_pid(&params))
			return (clean_fork_resources(pids, pipes, exec->pipe_count));
		current = current->next;
		i++;
	}
	return (cleanup_and_return(pipes, exec, pids));
}

int	exec_pipeline(t_pip *pipeline, t_exec *exec, int heredoc_fd)
{
	int	pipes[1024][2];

	exec->pipe_count = count_pipes(pipeline);
	if (setup_pipes(pipes, exec->pipe_count) == -1)
		return (1);
	return (exec_pipeline_pids(pipeline, exec, pipes, heredoc_fd));
}
