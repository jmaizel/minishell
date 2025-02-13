/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:04:22 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/13 21:04:43 by cdedessu         ###   ########.fr       */
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

static void	update_pwd_vars(t_tools *tools)
{
	char	current_pwd[PATH_MAX];
	char	*new_pwd;
	int		pwd_idx;

	pwd_idx = find_env_var(tools->env, "PWD");
	if (pwd_idx != -1 && getcwd(current_pwd, PATH_MAX))
	{
		new_pwd = ft_strjoin("PWD=", current_pwd);
		if (new_pwd)
		{
			tools->env[pwd_idx] = new_pwd;
		}
	}
}

int	builtin_cd(t_tools *tools, char **args)
{
	char	*path;
	char	*expanded_path;
	int		ret;

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
			return (ft_printf("minishell: cd: HOME not set\n"), 1);
		expanded_path = ft_strdup(path);
	}
	else
		expanded_path = expand_home_path(args[1]);
	if (!expanded_path)
		return (1);
	ret = chdir(expanded_path);
	if (ret == -1)
	{
		ft_printf("minishell: cd: %s: %s\n", expanded_path, strerror(errno));
		free(expanded_path);
		return (1);
	}
	free(expanded_path);
	update_pwd_vars(tools);
	return (0);
}
