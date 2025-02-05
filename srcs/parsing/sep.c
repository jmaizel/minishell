/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:17:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/03 12:23:51 by jacobmaizel      ###   ########.fr       */
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
		// Ne pas libérer current->redirection ici
		// car il sera libéré dans free_cmd_args
		free(current);
		current = next;
	}
	if (cell->cmd_sep)
		free(cell->cmd_sep);
	free(cell);
}

static int	is_invalid_char(char c)
{
	return (c == '\\' || c == ';');
}

/*
** check_invalid_chars: Vérifie les caractères invalides dans la commande
** Retourne 1 si un caractère invalide est trouvé hors guillemets
** Retourne 0 si la commande est valide
*/

int	check_invalid_chars(const char *cmd)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	if (!cmd)
		return (1);
	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (cmd[i])
	{
		if (!in_quotes && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			in_quotes = 1;
			quote_type = cmd[i];
		}
		else if (in_quotes && cmd[i] == quote_type)
			in_quotes = 0;
		else if (!in_quotes && is_invalid_char(cmd[i]))
			return (1);
		i++;
	}
	if (in_quotes)
		return (1);
	return (0);
}
