/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:41:03 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 17:41:27 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	perror("pwd");
	return (1);
}

int	builtin_export(t_simple_cmds *cmd, t_tools *tools)
{
	int	i;

	if (!cmd->str[1])
	{
		ft_putendl_fd("Usage: export VAR=VALUE", STDERR_FILENO);
		return (1);
	}
	i = 1;
	while (cmd->str[i])
	{
		add_env_var(cmd->str[i], &tools->env);
		i++;
	}
	return (0);
}

int	builtin_unset(t_simple_cmds *cmd, t_tools *tools)
{
	int	i;

	if (!cmd->str[1])
	{
		ft_putendl_fd("Usage: unset VAR", STDERR_FILENO);
		return (1);
	}
	i = 1;
	while (cmd->str[i])
	{
		remove_env_var(cmd->str[i], &tools->env);
		i++;
	}
	return (0);
}
