/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:41:59 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/18 15:54:49 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	close_unused_pipes(int prev_pipe, int pipe_fds[2])
{
	if (prev_pipe != -1)
		close(prev_pipe);
	close(pipe_fds[1]);
}

static void	handle_child_process(t_simple_cmds *cmd, int prev_pipe,
	int pipe_fds[2], t_tools *tools)
{
	if (prev_pipe != -1 && dup2(prev_pipe, STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	if (cmd->next && dup2(pipe_fds[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	close_unused_pipes(prev_pipe, pipe_fds);
	apply_redirections(cmd);
	execute_simple_command(cmd, tools);
	exit(tools->exit_code);
}

void	execute_pipeline(t_simple_cmds *cmd, t_tools *tools)
{
	int		pipe_fds[2];
	int		prev_pipe;
	pid_t	pid;

	prev_pipe = -1;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fds) == -1)
		{
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			handle_child_process(cmd, prev_pipe, pipe_fds, tools);
		if (prev_pipe != -1)
			close(prev_pipe);
		prev_pipe = pipe_fds[0];
		if (cmd->next)
			close(pipe_fds[1]);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}
