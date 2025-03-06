/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:12:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/06 18:02:51 by jmaizel          ###   ########.fr       */
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

static void	print_numeric_error(char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

static int	handle_too_many_args(t_tools *tools)
{
	ft_putendl_fd("exit: too many arguments", 2);
	tools->exit_code = 1;
	return (1);
}

static int	get_exit_status(t_tools *tools, char **argv)
{
	int	exit_status;

	if (!argv[1])
		return (tools->exit_code);
	if (!is_numeric(argv[1]))
	{
		print_numeric_error(argv[1]);
		return (2);
	}
	exit_status = ft_atoi(argv[1]) & 255;
	if (argv[2])
		return (handle_too_many_args(tools));
	return (exit_status);
}

int	builtin_exit(t_tools *tools, char **argv, int in_pipeline)
{
	int	exit_status;

	if (in_pipeline == 0)
		ft_putendl_fd("exit", 1);
	exit_status = get_exit_status(tools, argv);
	tools->exit_code = exit_status;
	if (in_pipeline == 0 && exit_status != 1)
	{
		cleanup_minishell(tools);
		exit(exit_status);
	}
	return (exit_status);
}