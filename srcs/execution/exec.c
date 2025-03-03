/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:23:52 by jmaizel          ###   ########.fr       */
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

static void	expand_commands(t_pip *current, t_tools *tools)
{
	char	*expanded_cmd;

	while (current)
	{
		expanded_cmd = expand_str(current->cmd_pipe, tools);
		if (expanded_cmd)
		{
			free(current->cmd_pipe);
			current->cmd_pipe = expanded_cmd;
		}
		if (!current->redirection)
			current->redirection = parse_redir(current->cmd_pipe);
		current = current->next;
	}
}

static int	handle_heredoc_setup(t_pip *pip_cell, t_exec *exec)
{
	t_parsed_cmd	*redir;
	int				heredoc_fd;

	redir = pip_cell->redirection;
	heredoc_fd = -1;
	if (redir && redir->heredoc_count > 0)
	{
		heredoc_fd = handle_heredoc(redir, exec);
		if (heredoc_fd == -1)
			return (-1);
	}
	return (heredoc_fd);
}

static int	execute_pipeline_or_simple(t_sep *cell, t_exec *exec, int heredoc_fd)
{
	int				ret;
	t_parsed_cmd	*redir;

	ret = 0;
	if (exec->pipe_count > 0)
	{
		ret = exec_pipeline(cell->pipcell, exec, heredoc_fd);
		if (heredoc_fd != -1)
			close(heredoc_fd);
	}
	else
	{
		redir = cell->pipcell->redirection;
		if (redir && redir->cmd && *redir->cmd)
		{
			ret = exec_with_redirections(cell->pipcell, exec, heredoc_fd);
		}
		else if (heredoc_fd != -1)
			close(heredoc_fd);
	}
	return (ret);
}

int	exec_commands(t_sep *cell, t_tools *tools)
{
	t_exec	exec;
	int		ret;
	int		heredoc_fd;

	if (!cell || !cell->pipcell)
		return (1);
	init_exec_struct(&exec, tools);
	exec.pipe_count = count_pipes(cell->pipcell);
	expand_commands(cell->pipcell, tools);
	heredoc_fd = handle_heredoc_setup(cell->pipcell, &exec);
	if (heredoc_fd == -1)
		return (1);
	ret = execute_pipeline_or_simple(cell, &exec, heredoc_fd);
	if (exec.cmd_paths)
		free_str_array(exec.cmd_paths);
	return (ret);
}

int	exec_with_redirections(t_pip *pip_cell, t_exec *exec, int heredoc_fd)
{
	int		ret;
	char	*expanded_cmd;

	if (heredoc_fd != -1)
	{
		exec->process.stdin_backup = dup(STDIN_FILENO);
		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
		{
			close(heredoc_fd);
			return (1);
		}
		close(heredoc_fd);
	}
	expanded_cmd = expand_str(pip_cell->redirection->cmd, exec->tools);
	if (expanded_cmd)
	{
		free(pip_cell->redirection->cmd);
		pip_cell->redirection->cmd = expanded_cmd;
	}
	ret = exec_simple_cmd(pip_cell, exec);
	if (heredoc_fd != -1)
		restore_redirections(&exec->process);
	return (ret);
}
