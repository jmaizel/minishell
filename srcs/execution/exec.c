/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 17:14:41 by jmaizel          ###   ########.fr       */
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

static void	expand_cmd_and_check_redir(t_pip *current, t_tools *tools)
{
	char			*expanded_cmd;
	t_parsed_cmd	*old_redir;

	old_redir = current->redirection;
	expanded_cmd = expand_str(current->cmd_pipe, tools);
	if (expanded_cmd)
	{
		free(current->cmd_pipe);
		current->cmd_pipe = expanded_cmd;
	}
	if (!current->redirection)
		current->redirection = parse_redir(current->cmd_pipe);
	else if (old_redir != current->redirection && old_redir)
		free_parsed_cmd(old_redir);
}

static void	expand_commands(t_pip *current, t_tools *tools)
{
	while (current)
	{
		expand_cmd_and_check_redir(current, tools);
		current = current->next;
	}
}

static int	manage_heredoc(t_parsed_cmd *redir, t_exec *exec, int *heredoc_fd)
{
	if (redir && redir->heredoc_count > 0)
	{
		*heredoc_fd = handle_heredoc(redir, exec);
		if (*heredoc_fd == -1)
			return (0);
	}
	return (1);
}

static int	prepare_heredoc_fd(t_exec *exec, int *heredoc_fd)
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
	return (1);
}

static int	expand_and_check_redirection(t_parsed_cmd *redir, t_exec *exec)
{
	char	*expanded_cmd;

	expanded_cmd = expand_str(redir->cmd, exec->tools);
	if (expanded_cmd)
	{
		free(redir->cmd);
		redir->cmd = expanded_cmd;
	}
	return (1);
}

static int	handle_heredoc_and_expand(t_sep *cell, t_exec *exec,
		int *heredoc_fd)
{
	t_parsed_cmd	*redir;

	redir = find_redir(cell->pipcell);
	if (!manage_heredoc(redir, exec, heredoc_fd))
		return (0);
	if (exec->pipe_count <= 0 && redir && redir->cmd && *redir->cmd)
	{
		if (!prepare_heredoc_fd(exec, heredoc_fd))
			return (0);
		if (!expand_and_check_redirection(redir, exec))
			return (0);
	}
	return (1);
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
