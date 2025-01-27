/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:34 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 21:10:39 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static int	update_env_var(char *key, char *value, char ***env)
{
	char	*var;
	int		ret;

	var = ft_strjoin(key, "=");
	if (!var)
		return (ERR_MALLOC_FAILURE);
	var = ft_strjoin_free(var, value, 1, 0);
	if (!var)
		return (ERR_MALLOC_FAILURE);
	ret = add_env_var(var, env);
	free(var);
	return (ret);
}

int	builtin_cd(t_simple_cmds *cmd, t_tools *tools)
{
	char	*path;
	char	*oldpwd;
	char	*cwd;

	if (!cmd->str[1] || ft_strcmp(cmd->str[1], "~") == 0)
		path = get_env_var("HOME", tools->env);
	else if (ft_strcmp(cmd->str[1], "-") == 0)
		path = get_env_var("OLDPWD", tools->env);
	else
		path = cmd->str[1];
	if (!path || chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(path);
		return (ERR_INVALID_CMD);
	}
	oldpwd = get_env_var("PWD", tools->env);
	if (update_env_var("OLDPWD", oldpwd ? oldpwd : "", &tools->env) != SUCCESS)
		return (ERR_MALLOC_FAILURE);
	cwd = getcwd(NULL, 0);
	if (update_env_var("PWD", cwd, &tools->env) != SUCCESS)
	{
		free(cwd);
		return (ERR_MALLOC_FAILURE);
	}
	free(cwd);
	return (SUCCESS);
}
