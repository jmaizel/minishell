/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 21:11:26 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	init_exec_struct(t_exec *exec, t_tools *tools)
{
	exec->tools = tools;
	exec->exit_status = 0;
	exec->pipe_count = 0;
	exec->process.stdin_backup = -1;
	exec->process.stdout_backup = -1;
	exec->process.pid = -1;
	exec->cmd_paths = get_env_paths(tools->env, "PATH");
}

static void	execute_pipeline(t_sep *cell, t_exec *exec, int heredoc_fd)
{
	exec_pipeline(cell->pipcell, exec, heredoc_fd);
	if (heredoc_fd != -1)
		close(heredoc_fd);
}

static void	execute_simple_command(t_sep *cell, t_exec *exec, int heredoc_fd)
{
	exec_simple_cmd(cell->pipcell, exec);
	if (heredoc_fd != -1)
		restore_redirections(&exec->process);
}

static void	cleanup_redirections(t_pip *current)
{
	t_pip	*temp;

	temp = current;
	while (temp)
	{
		if (temp->redirection)
		{
			free_parsed_cmd(temp->redirection);
			temp->redirection = NULL;
		}
		temp = temp->next;
	}
}

int	exec_commands(t_sep *cell, t_tools *tools)
{
	t_exec	exec;
	int		heredoc_fd;

	if (!cell || !cell->pipcell)
		return (1);
	init_exec_struct(&exec, tools);
	exec.pipe_count = count_pipes(cell->pipcell);
	expand_commands(cell->pipcell, tools);
	heredoc_fd = -1;
	if (!handle_heredoc_and_expand(cell, &exec, &heredoc_fd))
	{
		cleanup_redirections(cell->pipcell);
		if (exec.cmd_paths)
			free_str_array(exec.cmd_paths);
		return (1);
	}
	if (exec.pipe_count > 0)
		execute_pipeline(cell, &exec, heredoc_fd);
	else
		execute_simple_command(cell, &exec, heredoc_fd);
	cleanup_redirections(cell->pipcell);
	if (exec.cmd_paths)
		free_str_array(exec.cmd_paths);
	return (0);
}
