/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:04:22 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/25 17:48:21 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

static char	*expand_home_path(char *path)
{
	char	*home;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	home = getenv("HOME");
	if (!home)
		return (ft_strdup(path));
	if (path[1] == '\0')
		return (ft_strdup(home));
	else if (path[1] == '/')
		return (ft_strjoin(home, path + 1));
	return (ft_strdup(path));
}

static int	update_env_var(t_tools *tools, const char *name, const char *value)
{
	char	*new_var;
	int		idx;

	new_var = ft_strjoin(name, value);
	if (!new_var)
		return (0);
	idx = find_env_var(tools->env, name);
	if (idx != -1)
		tools->env[idx] = new_var;
	return (1);
}

static void	update_pwd_vars(t_tools *tools, const char *old_pwd)
{
	char	current_pwd[PATH_MAX];

	if (!getcwd(current_pwd, PATH_MAX))
	{
		return ;
	}
	if (old_pwd)
		update_env_var(tools, "OLDPWD=", old_pwd);
	update_env_var(tools, "PWD=", current_pwd);
}

static char	*get_cd_path(t_tools *tools, char **args)
{
	char	*path;
	int		oldpwd_idx;

	if (!args[1])
	{
		path = getenv("HOME");
		if (path == NULL)
			return (NULL);
		return (ft_strdup(path));
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		oldpwd_idx = find_env_var(tools->env, "OLDPWD");
		if (oldpwd_idx == -1)
			return (NULL);
		path = ft_strdup(tools->env[oldpwd_idx] + 7);
		if (path)
			ft_printf("%s\n", path);
		return (path);
	}
	return (expand_home_path(args[1]));
}

int	builtin_cd(t_tools *tools, char **args)
{
	char	*expanded_path;
	char	old_pwd[PATH_MAX];
	int		ret;

	if (!getcwd(old_pwd, PATH_MAX))
		return (1);
	expanded_path = get_cd_path(tools, args);
	if (expanded_path == NULL)
	{
		if (!args[1])
			ft_printf("minishell: cd: HOME not set\n");
		if (args[1] && ft_strcmp(args[1], "-") == 0)
			ft_printf("minishell: cd: OLDPWD not set\n");
		return (1);
	}
	ret = chdir(expanded_path);
	if (ret == -1)
	{
		ft_printf("minishell: cd: %s: %s\n", expanded_path, strerror(errno));
		free(expanded_path);
		return (1);
	}
	update_pwd_vars(tools, old_pwd);
	free(expanded_path);
	return (0);
}
