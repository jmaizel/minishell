/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:04:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/03 12:52:47 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction qui va parser les pipes :
// 1. fait un split pour separer les commandes par pipes
// 2. ensuite on parcours ces commandes et on creer une cellule pour chaque commande
// 3. on parser les redirections
// 4. on finit par stcoker la liste des pipes dans la cellule
void	parse_pipes(t_sep *cell)
{
	char	**pipe_commands;
	t_pip	*current;
	int		i;
	t_pip	*new_pipe;

	if (!cell || !cell->cmd_sep)
		return ;
	pipe_commands = ft_split_pipes(cell->cmd_sep, '|');
	if (!pipe_commands)
		return ;
	i = 0;
	cell->pipcell = NULL;
	while (pipe_commands[i])
	{
		new_pipe = malloc(sizeof(t_pip));
		if (!new_pipe)
		{
			free_str_array(pipe_commands);
			return ;
		}
		new_pipe->cmd_pipe = ft_strdup(pipe_commands[i]);
		new_pipe->redirection = NULL;
		new_pipe->next = NULL;
		new_pipe->prev = NULL;
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
		i++;
	}
	free_str_array(pipe_commands);
}
