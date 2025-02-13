/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:45:41 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/13 21:06:05 by cdedessu         ###   ########.fr       */
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
		while (str[i] == 'n')
			i++;
		if (str[i] == 'e')
		{
			while (str[i] == 'e')
				i++;
		}
	}
	return (1);
}

int	builtin_echo(t_tools *tools, char **args)
{
	int	i;
	int	n_flag;

	(void)tools;
	n_flag = 0;
	i = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!n_flag)
		ft_printf("\n");
	return (0);
}
