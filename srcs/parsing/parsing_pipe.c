/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:04:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:10:27 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pip	*create_pipe_node(char *cmd_content)
{
	t_pip	*new_pipe;

	new_pipe = malloc(sizeof(t_pip));
	if (!new_pipe)
		return (NULL);
	new_pipe->cmd_pipe = ft_strdup(cmd_content);
	new_pipe->redirection = NULL;
	new_pipe->next = NULL;
	new_pipe->prev = NULL;
	return (new_pipe);
}

static void	add_pipe_to_cell(t_sep *cell, t_pip *new_pipe)
{
	t_pip	*current;

	if (!cell->pipcell)
		cell->pipcell = new_pipe;
	else
	{
		current = cell->pipcell;
		while (current->next)
			current = current->next;
		current->next = new_pipe;
		new_pipe->prev = current;
	}
}

void	parse_pipes(t_sep *cell)
{
	char	**pipe_commands;
	t_pip	*new_pipe;
	int		i;

	if (!cell || !cell->cmd_sep)
		return ;
	pipe_commands = ft_split_pipes(cell->cmd_sep, '|');
	if (!pipe_commands)
		return ;
	i = 0;
	cell->pipcell = NULL;
	while (pipe_commands[i])
	{
		new_pipe = create_pipe_node(pipe_commands[i]);
		if (!new_pipe)
		{
			free_str_array(pipe_commands);
			return ;
		}
		add_pipe_to_cell(cell, new_pipe);
		i++;
	}
	free_str_array(pipe_commands);
}
