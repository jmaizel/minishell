/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:41:59 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 16:50:39 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	cleanup_pipe_array(int **pipes, int count)
{
	int	i;

	if (!pipes || !*pipes)
		return ;
	i = 0;
	while (i < count)
	{
		close((*pipes)[i * 2]);
		close((*pipes)[i * 2 + 1]);
		i++;
	}
	free(*pipes);
	*pipes = NULL;
}

static int	*create_pipes(int count)
{
	int	*pipes;
	int	i;

	pipes = malloc(sizeof(int) * 2 * count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (pipe(pipes + (i * 2)) == -1)
		{
			cleanup_pipe_array(&pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

static void	setup_pipe_fds(int *pipes, int i, int pipe_count)
{
	int	j;

	if (i > 0)
	{
		if (dup2(pipes[(i - 1) * 2], STDIN_FILENO) == -1)
			handle_error("dup2 failed");
	}
	if (i < pipe_count)
	{
		if (dup2(pipes[i * 2 + 1], STDOUT_FILENO) == -1)
			handle_error("dup2 failed");
	}
	j = 0;
	while (j < pipe_count * 2)
		close(pipes[j++]);
}

static void	execute_children(t_simple_cmds *cmd, t_tools *tools, int *pipes,
		pid_t *pids)
{
	int				i;
	int				pipe_count;
	t_simple_cmds	*tmp;

	pipe_count = 0;
	tmp = cmd;
	while (tmp->next)
	{
		pipe_count++;
		tmp = tmp->next;
	}
	tmp = cmd;
	i = 0;
	while (tmp)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			handle_error("fork failed");
		if (pids[i] == 0)
		{
			setup_child_signals();
			setup_pipe_fds(pipes, i, pipe_count);
			execute_simple_command(tmp, tools);
			exit(tools->exit_code);
		}
		tmp = tmp->next;
		i++;
	}
}

void	execute_pipeline(t_simple_cmds *cmd, t_tools *tools)
{
	int		pipe_count;
	int		*pipes;
	pid_t	*pids;
	int		i;
	int		status;

	pipe_count = 0;
	while (cmd->next && ++pipe_count)
		cmd = cmd->next;
	pipes = create_pipes(pipe_count);
	if (!pipes)
		handle_error("pipe creation failed");
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	if (!pids)
	{
		cleanup_pipe_array(&pipes, pipe_count);
		handle_error("malloc failed");
	}
	execute_children(cmd, tools, pipes, pids);
	cleanup_pipe_array(&pipes, pipe_count);
	i = -1;
	while (++i < pipe_count + 1)
	{
		waitpid(pids[i], &status, 0);
		if (i == pipe_count)
			update_exit_status(tools, status);
	}
	free(pids);
}
