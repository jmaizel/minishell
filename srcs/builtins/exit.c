/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:12:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/12 08:24:16 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_numeric(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_tools *tools, char **argv)
{
	int	exit_status;

	ft_putendl_fd("exit", 1);
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit_status = 2;
		}
		else
		{
			exit_status = ft_atoi(argv[1]);
			if (argv[2])
			{
				ft_putendl_fd("exit: too many arguments", 2);
				return (1);
			}
		}
	}
	else
		exit_status = tools->exit_code;
	cleanup_minishell(tools);
	exit(exit_status);
	return (exit_status);
}
