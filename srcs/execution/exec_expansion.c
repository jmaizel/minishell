/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:01:10 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 21:05:23 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	expand_cmd_and_check_redir(t_pip *current, t_tools *tools)
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

void	expand_commands(t_pip *current, t_tools *tools)
{
	while (current)
	{
		expand_cmd_and_check_redir(current, tools);
		current = current->next;
	}
}

int	expand_and_check_redirection(t_parsed_cmd *redir, t_exec *exec)
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
