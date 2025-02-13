/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 15:42:20 by jmaizel          ###   ########.fr       */
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

int	exec_commands(t_sep *cell, t_tools *tools)
{
	t_exec	exec;
	int		ret;
	char	*expanded_cmd;

	if (!cell || !cell->pipcell)
		return (1);
	expanded_cmd = expand_str(cell->pipcell->cmd_pipe, tools);
	if (expanded_cmd)
	{
		free(cell->pipcell->cmd_pipe);
		cell->pipcell->cmd_pipe = expanded_cmd;
	}
	init_exec_struct(&exec, tools);
	exec.pipe_count = count_pipes(cell->pipcell);
	if (exec.pipe_count > 0)
		ret = exec_pipeline(cell->pipcell, &exec);
	else
	{
		if (!cell->pipcell->redirection)
			cell->pipcell->redirection = parse_redir(cell->pipcell->cmd_pipe);
		if (cell->pipcell->redirection && cell->pipcell->redirection->cmd)
		{
			expanded_cmd = expand_str(cell->pipcell->redirection->cmd, tools);
			if (expanded_cmd)
			{
				free(cell->pipcell->redirection->cmd);
				cell->pipcell->redirection->cmd = expanded_cmd;
			}
		}
		ret = exec_simple_cmd(cell->pipcell, &exec);
	}
	if (exec.cmd_paths)
		free_str_array(exec.cmd_paths);
	return (ret);
}
