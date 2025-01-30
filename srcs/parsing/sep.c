/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:17:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/30 13:31:48 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction qui creer une cellule de la liste chainnee
t_sep	*create_cell(char *cmd_sep)
{
	t_sep	*cell;

	cell = malloc(sizeof(t_sep));
	if (!cell)
		return (NULL);
	cell->prev = NULL;
	cell->next = NULL;
	cell->pipcell = NULL;
	cell->cmd_sep = cmd_sep;
	return (cell);
}

// Cette fonction ajoutera une cellule dans la liste chaînée à la position spécifiée.
// Si la liste est vide,
// la nouvelle cellule sera simplement le premier élément de la liste.

t_sep	*add_cell(t_sep *list, char *cmd_sep, int pos)
{
	t_sep	*prec;
	t_sep	*cur;
	t_sep	*cell;
	int		i;

	cell = create_cell(cmd_sep);
	if (!list)
		return (cell);
	cur = list;
	i = 0;
	while (i < pos && cur != NULL)
	{
		prec = cur;
		cur = cur->next;
		i++;
	}
	if (prec != NULL)
	{
		prec->next = cell;
		cell->prev = prec;
	}
	if (cur != NULL)
	{
		cell->next = cur;
		cur->prev = cell;
	}
	return (list);
}
