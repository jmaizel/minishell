/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:08:04 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 21:12:23 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

t_parsed_cmd	*find_redir(t_pip *current)
{
	t_parsed_cmd	*redir;

	redir = NULL;
	if (current->redirection)
		redir = current->redirection;
	return (redir);
}

int	manage_heredoc(t_parsed_cmd *redir, t_exec *exec, int *heredoc_fd)
{
	if (redir && redir->heredoc_count > 0)
	{
		*heredoc_fd = handle_heredoc(redir, exec);
		if (*heredoc_fd == -1)
			return (0);
	}
	return (1);
}

int	prepare_heredoc_fd(t_exec *exec, int *heredoc_fd)
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

int	handle_heredoc_and_expand(t_sep *cell, t_exec *exec,
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
