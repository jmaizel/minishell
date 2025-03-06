/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:17:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/03/03 14:49:47 by jmaizel          ###   ########.fr       */
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

static void	find_insert_position(t_sep **cur, t_sep **prec, int pos)
{
	int	i;

	i = 0;
	while (i < pos && *cur != NULL)
	{
		*prec = *cur;
		*cur = (*cur)->next;
		i++;
	}
}

t_sep	*add_cell(t_sep *list, char *cmd_sep, int pos)
{
	t_sep	*prec;
	t_sep	*cur;
	t_sep	*cell;

	cell = create_cell(cmd_sep);
	if (!list)
		return (cell);
	cur = list;
	prec = NULL;
	find_insert_position(&cur, &prec, pos);
	if (prec)
	{
		prec->next = cell;
		cell->prev = prec;
	}
	if (cur)
	{
		cell->next = cur;
		cur->prev = cell;
	}
	return (list);
}

static int	is_invalid_char(char c)
{
	return (c == '\\' || c == ';');
}

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
