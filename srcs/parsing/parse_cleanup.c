/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:26:45 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_redir_arrays(t_parsed_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->input_count)
		free(cmd->input_file[i++]);
	free(cmd->input_file);
	i = 0;
	while (i < cmd->output_count)
		free(cmd->output_file[i++]);
	free(cmd->output_file);
	i = 0;
	while (i < cmd->append_count)
		free(cmd->append_file[i++]);
	free(cmd->append_file);
	i = 0;
	while (i < cmd->heredoc_count)
		free(cmd->heredoc_delim[i++]);
	free(cmd->heredoc_delim);
}

void	free_parsed_cmd(t_parsed_cmd *cmd)
{
	if (!cmd)
		return ;
	free(cmd->full_cmd);
	free(cmd->cmd);
	free_redir_arrays(cmd);
	free(cmd);
}

void	free_cell(t_sep *cell)
{
	t_pip	*current;
	t_pip	*next;

	if (!cell)
		return ;
	current = cell->pipcell;
	while (current)
	{
		next = current->next;
		if (current->cmd_pipe)
			free(current->cmd_pipe);
		free(current);
		current = next;
	}
	if (cell->cmd_sep)
		free(cell->cmd_sep);
	free(cell);
}
