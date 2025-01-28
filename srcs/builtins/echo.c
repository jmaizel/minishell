/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/28 10:55:20 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static int	is_option_n(char *arg)
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

int	builtin_echo(t_simple_cmds *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (cmd->str[i] && is_option_n(cmd->str[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->str[i])
	{
		ft_putstr_fd(cmd->str[i], STDOUT_FILENO);
		if (cmd->str[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
