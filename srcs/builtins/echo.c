/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:45:41 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/12 13:55:02 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

static int	is_valid_n_option(char *str)
{
	int	i;


	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
int	builtin_echo(t_tools *tools, char **args)
{
	int i;
	int n_flag;

	(void)tools;
	n_flag = 0;
	i = 1;

	// on verifife si il ya le -n ou -nn ou - nnn etc
	while (args[i] && is_valid_n_option(args[i]))
	{
		n_flag = 1;
		i++;
	}
	// affichage des arguments
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	// on ajoute un retour a la ligne sauf si -n est present
	if (!n_flag)
		ft_printf("\n");
	return (0);
}