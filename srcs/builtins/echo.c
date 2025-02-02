/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/02 19:35:49 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_option_n(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	print_arguments(char **args, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(t_parsed_cmd *cmd)
{
	char	**args;
	int		i;
	int		newline;

	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (1);
	i = 1;
	newline = 1;
	while (args[i] && is_option_n(args[i]))
	{
		newline = 0;
		i++;
	}
	print_arguments(args, i);
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	free_str_array(args);
	return (0);
}
