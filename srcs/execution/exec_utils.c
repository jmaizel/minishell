/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:02 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/11 13:18:03 by jmaizel          ###   ########.fr       */
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

static void	handle_exited_process(t_exec *exec, int status)
{
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			exec->exit_status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
		{
			exec->exit_status = 131;
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		}
		else
			exec->exit_status = 128 + WTERMSIG(status);
	}
}

static int	check_signal_interrupt(int status)
{
	return (WTERMSIG(status) == SIGINT);
}

void	wait_all_processes(t_exec *exec, int process_count)
{
	int		i;
	int		status;
	int		sigint_detected;
	int		last_exit_status;
	pid_t	pid;

	i = 0;
	sigint_detected = 0;
	last_exit_status = 0;
	while (i < process_count)
	{
		pid = wait(&status);
		if (pid > 0)
		{
			if (i == process_count - 1)
				update_exit_status(status, &last_exit_status);
			handle_exited_process(exec, status);
			if (check_signal_interrupt(status))
				sigint_detected = 1;
			i++;
		}
	}
	exec->tools->exit_code = last_exit_status;
	if (sigint_detected)
		ft_putchar_fd('\n', STDERR_FILENO);
}
