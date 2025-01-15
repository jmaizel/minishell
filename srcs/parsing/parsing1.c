/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:19:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/15 14:23:22 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cette fonction : on prend en param dq et sq pour voir si on est dans une " et
// sa pour savoir si on est dans une '.
// explication de l algo , on regarde si cest un /' ou un " et si sq et dq sont false, cad qu aucune des deux possibilite n est ouverte. 
// si c est

void	quoting_choice(bool *dq, bool *sq, int *index, char c)
{
	if ((c == '\'' || c == '"') && !*sq && !*dq)
	{
		if (c == '\'' && !*dq)
			*sq = true;
		else if (c == '"' && !*sq)
			*dq = true;
		if (index)
			++(*index);
	}
	else if ((c == '\'' || c == '"'))
	{
		if (c == '\'' && !*dq && *sq)
			*sq = false;
		else if (c == '"' && !*sq && *dq)
			*dq = false;
		if (index)
			++(*index);
	}
}

int	is_quote_closed(char *str)
{
	bool	dq;
	bool	sq;
	int		i;

	dq = false;
	sq = false;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && dq == false)
		{
			sq = !sq;
		}
		else if (str[i] == '\"' && sq == false)
		{
			dq = !dq;
		}
		i++;
	}
	if (dq || sq)
		return (0);
	else
		return (1);
}
