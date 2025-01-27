/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:34 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 11:43:36 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static char	*get_home_dir(char **env)
{
	char	*home;

	home = get_env_var("HOME", env);
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (home);
}

int	builtin_cd(t_simple_cmds *cmd, t_tools *tools)
{
	char	*path;

	if (!cmd->str[1])
	{
		path = get_home_dir(tools->env);
		if (!path)
			return (1);
	}
	else
		path = cmd->str[1];

	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		tools->exit_code = 1;
		return (1);
	}

	tools->exit_code = 0;
	return (0);
}