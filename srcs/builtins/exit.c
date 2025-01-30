/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:11:03 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/30 16:51:03 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_numeric(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_simple_cmds *cmd, t_tools *tools)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!cmd || !cmd->str || !tools)
		return (1);
	if (cmd->str[1] && cmd->str[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (cmd->str[1])
	{
		if (!is_numeric(cmd->str[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->str[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit_code = 2;
		}
		else
			exit_code = ft_atoi(cmd->str[1]);
	}
	else
		exit_code = tools->exit_code;
	exit(exit_code % 256);
}
