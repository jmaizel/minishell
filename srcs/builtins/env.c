/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:01:13 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/28 15:52:26 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	builtin_env(t_simple_cmds *cmd, t_tools *tools)
{
	int	i;

	if (cmd->str[1])
	{
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		return (ERR_INVALID_CMD);
	}
	if (!tools->env)
	{
		ft_putendl_fd("env: environment is empty", STDERR_FILENO);
		return (ERR_INVALID_CMD);
	}
	i = 0;
	while (tools->env[i])
	{
		ft_putendl_fd(tools->env[i], STDOUT_FILENO);
		i++;
	}
	return (SUCCESS);
}
