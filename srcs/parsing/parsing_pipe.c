/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:04:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/30 13:29:49 by jmaizel          ###   ########.fr       */
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
	int		i;
	t_pip	*pipe_list;
	t_pip	*pipe_cell;
	t_pip	*current;

	pipe_list = NULL;
	pipe_commands = ft_split_pipes(cell->cmd_sep, '|');
	if (!pipe_commands)
		return ;
	i = 0;
	while (pipe_commands[i])
	{
		pipe_cell = malloc(sizeof(t_pip));
		if (!pipe_cell)
			return ;
		pipe_cell->cmd_pipe = ft_strdup(pipe_commands[i]);
		if (!pipe_cell->cmd_pipe)
		{
			free(pipe_cell);
			return ;
		}
		// Parser les redirections
		pipe_cell->redirection = parse_command(pipe_cell->cmd_pipe);
		pipe_cell->next = NULL;
		pipe_cell->prev = NULL;
		// Ajouter à la liste des pipes
		if (!pipe_list)
			pipe_list = pipe_cell;
		else
		{
			current = pipe_list;
			while (current->next)
				current = current->next;
			current->next = pipe_cell;
			pipe_cell->prev = current;
		}
		i++;
	}
	cell->pipcell = pipe_list;
	free_str_array(pipe_commands);
}
