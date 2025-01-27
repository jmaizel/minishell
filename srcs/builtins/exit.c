/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:44:36 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 11:44:37 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static int	is_valid_number(char *str)
{
	int	i;

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

int	builtin_exit(t_simple_cmds *cmd)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!cmd->str[1])
		exit(0);
	if (!is_valid_number(cmd->str[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->str[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(2);
	}
	if (cmd->str[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	exit_code = ft_atoi(cmd->str[1]);
	exit(exit_code);
}