/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_return.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:05:53 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/12 15:08:15 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_termination_signals(int signal)
{
	if (signal == SIGINT)
		ft_putchar_fd('\n', STDERR_FILENO);
	else if (signal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
}

static void	set_exit_code(t_exec *exec, int status)
{
	if (WIFEXITED(status))
		exec->tools->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			exec->tools->exit_code = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			exec->tools->exit_code = 131;
		else
			exec->tools->exit_code = 128 + WTERMSIG(status);
		handle_termination_signals(WTERMSIG(status));
	}
}

static void	wait_remaining_processes(pid_t *pids, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}

int	cleanup_and_return(int pipes[][2], t_exec *exec, pid_t *pids)
{
	int	last_pid_index;
	int	status;

	close_all_pipes(pipes, exec->pipe_count);
	last_pid_index = exec->pipe_count;
	waitpid(pids[last_pid_index], &status, 0);
	set_exit_code(exec, status);
	wait_remaining_processes(pids, exec->pipe_count);
	free(pids);
	exec->exit_status = exec->tools->exit_code;
	return (exec->exit_status);
}
