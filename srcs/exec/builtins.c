/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:20:29 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 13:43:12 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	builtin_echo(t_simple_cmds *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->str[1] && ft_strcmp(cmd->str[1], "-n") == 0)
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
	return (1);
}

int	builtin_cd(t_simple_cmds *cmd, t_tools *tools)
{
	if (!cmd->str[1])
	{
		ft_putendl_fd("cd: missing argument", STDERR_FILENO);
		tools->exit_code = 1;
		return (1);
	}
	if (chdir(cmd->str[1]) == -1)
	{
		perror("cd");
		tools->exit_code = 1;
	}
	else
		tools->exit_code = 0;
	return (1);
}

int	builtin_exit(t_simple_cmds *cmd)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd->str[1])
	{
		exit_code = ft_atoi(cmd->str[1]);
		exit(exit_code);
	}
	exit(0);
}

int	builtin_env(t_tools *tools)
{
	int	i;

	i = 0;
	while (tools->env[i])
	{
		ft_putendl_fd(tools->env[i], STDOUT_FILENO);
		i++;
	}
	return (1);
}
