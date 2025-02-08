/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:42:12 by cberganz          #+#    #+#             */
/*   Updated: 2025/02/08 12:43:46 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	setup_pipes(int prev_pipe[2], int next_pipe[2], int is_last)
{
	if (prev_pipe[0] != -1)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) == -1)
			exit(1);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (!is_last)
	{
		close(next_pipe[0]);
		if (dup2(next_pipe[1], STDOUT_FILENO) == -1)
			exit(1);
		close(next_pipe[1]);
	}
}

static void	execute_piped_cmd(t_pip *cmd, t_exec *exec, int prev_pipe[2],
		int is_last)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!is_last && pipe(pipe_fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		setup_pipes(prev_pipe, is_last ? (int[]){-1, -1} : pipe_fd, is_last);
		if (cmd->redirection)
			setup_redirections(cmd->redirection, &exec->process);
		exec_simple_cmd(cmd, exec);
		exit(exec->exit_status);
	}
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (!is_last)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
	exec->process.pid = pid;
}

int	exec_pipeline(t_pip *pipeline, t_exec *exec)
{
	t_pip	*current;
	int		prev_pipe[2];
	int		cmd_count;
	int		total_cmds;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	total_cmds = count_pipes(pipeline) + 1;
	cmd_count = 0;
	current = pipeline;

	while (current)
	{
		execute_piped_cmd(current, exec, prev_pipe, cmd_count == total_cmds - 1);
		cmd_count++;
		current = current->next;
	}

	wait_all_processes(exec, total_cmds);
	return (exec->exit_status);
}
