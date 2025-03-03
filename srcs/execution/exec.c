/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:39:20 by jmaizel          ###   ########.fr       */
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

static t_parsed_cmd	*find_redir(t_pip *current)
{
	t_parsed_cmd	*redir;

	redir = NULL;
	if (current->redirection)
		redir = current->redirection;
	return (redir);
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

static int	handle_heredoc_and_expand(t_sep *cell, t_exec *exec, int *heredoc_fd)
{
	t_parsed_cmd	*redir;
	char			*expanded_cmd;

	redir = find_redir(cell->pipcell);
	if (redir && redir->heredoc_count > 0)
	{
		*heredoc_fd = handle_heredoc(redir, exec);
		if (*heredoc_fd == -1)
			return (0);
	}
	if (exec->pipe_count <= 0 && redir && redir->cmd && *redir->cmd)
	{
		if (*heredoc_fd != -1)
		{
			exec->process.stdin_backup = dup(STDIN_FILENO);
			if (dup2(*heredoc_fd, STDIN_FILENO) == -1)
			{
				close(*heredoc_fd);
				return (0);
			}
			close(*heredoc_fd);
		}
		expanded_cmd = expand_str(redir->cmd, exec->tools);
		if (expanded_cmd)
		{
			free(redir->cmd);
			redir->cmd = expanded_cmd;
		}
	}
	return (1);
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
	heredoc_fd = -1;
	if (!handle_heredoc_and_expand(cell, &exec, &heredoc_fd))
		return (1);
	if (exec.pipe_count > 0)
	{
		ret = exec_pipeline(cell->pipcell, &exec, heredoc_fd);
		if (heredoc_fd != -1)
			close(heredoc_fd);
	}
	else
	{
		ret = exec_simple_cmd(cell->pipcell, &exec);
		if (heredoc_fd != -1)
			restore_redirections(&exec.process);
	}
	if (exec.cmd_paths)
		free_str_array(exec.cmd_paths);
	return (ret);
}
