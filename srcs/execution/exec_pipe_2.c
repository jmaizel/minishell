/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:29:25 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	setup_pipe_redirection(t_pip *cmd, t_exec *exec, 
									int i, int heredoc_fd)
{
	(void)cmd;
	if (heredoc_fd != -1 && i == 0)
	{
		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			exit(1);
		close(heredoc_fd);
	}
	else if (i > 0)
	{
		if (dup2(exec->pipe_fds[i - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (i < exec->pipe_count)
	{
		if (dup2(exec->pipe_fds[i][1], STDOUT_FILENO) == -1)
			exit(1);
	}
}

static pid_t	fork_and_execute(t_pip *cmd, t_exec *exec, 
								int i, int heredoc_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		setup_pipe_redirection(cmd, exec, i, heredoc_fd);
		close_all_pipes(exec->pipe_fds, exec->pipe_count);
		if (cmd->redirection && cmd->redirection->heredoc_count == 0)
		{
			if (setup_redirections(cmd->redirection, &exec->process, exec) == -1)
				exit(1);
		}
		execute_command(cmd, exec, i);
	}
	if (heredoc_fd != -1 && i == 0)
		close(heredoc_fd);
	return (pid);
}

static int	allocate_pipe_resources(t_exec *exec, pid_t **pids)
{
	*pids = malloc(sizeof(pid_t) * (exec->pipe_count + 1));
	if (!*pids)
	{
		close_all_pipes(exec->pipe_fds, exec->pipe_count);
		return (0);
	}
	return (1);
}

int	exec_pipeline(t_pip *pipeline, t_exec *exec, int heredoc_fd)
{
	t_pip	*current;
	pid_t	*pids;
	int		i;

	exec->pipe_count = count_pipes(pipeline);
	if (setup_pipes(exec->pipe_fds, exec->pipe_count) == -1)
		return (1);
	if (!allocate_pipe_resources(exec, &pids))
		return (1);
	i = 0;
	current = pipeline;
	while (current)
	{
		pids[i] = fork_and_execute(current, exec, i, heredoc_fd);
		if (pids[i] == -1)
		{
			free(pids);
			close_all_pipes(exec->pipe_fds, exec->pipe_count);
			return (1);
		}
		current = current->next;
		i++;
	}
	close_all_pipes(exec->pipe_fds, exec->pipe_count);
	wait_all_processes(exec, exec->pipe_count + 1);
	free(pids);
	return (exec->exit_status);
}
