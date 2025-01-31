/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redir_only.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:01:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 16:01:45 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include <stdbool.h>

static int	is_redirection_char(char c)
{
	return (c == '<' || c == '>');
}

bool	is_redirection_only(const char *str)
{
	int	i;

	if (!str)
		return (true); // Si la chaîne est NULL, on considère que c'est une erreur

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++; // Ignore les espaces au début

	// Vérifier si la chaîne ne contient que des redirections
	if (is_redirection_char(str[i]))
	{
		if (str[i] == str[i + 1]) // Cas "<<" ou ">>"
			i += 2;
		else
			i++;

		while (str[i] == ' ' || str[i] == '\t')
			i++; // Ignore les espaces après la redirection

		// Si on atteint la fin, alors il n'y a **que** une redirection
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}
