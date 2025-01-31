/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:25:48 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/31 15:45:12 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	ft_isspace(int c)
{
	if (c == 32)
		return (1);
	return (0);
}
#include "minishell.h"

int	is_valid_command(const char *cmd)
{
	int	i;

	i = 0;
	char quote = 0; 
	while (cmd[i])
	{
		if ((cmd[i] == '"' || cmd[i] == '\''))
		{
			if (quote == 0)
				quote = cmd[i];
			else if (quote == cmd[i])
				quote = 0;
		}
		else if ((cmd[i] == '\\' || cmd[i] == ';') && quote == 0)
		{
			printf("Erreur : caractère invalide '%c' détecté !\n", cmd[i]);
			return (0);
		}
		i++;
	}
	if (quote != 0)
	{
		printf("Erreur : guillemet non fermé !\n");
		return (0);
	}
	return (1);
}
