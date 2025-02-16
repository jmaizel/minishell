/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:04:22 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/15 10:22:34 by cdedessu         ###   ########.fr       */
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
	char	*new_oldpwd;
	int		pwd_idx;
	int		oldpwd_idx;

	pwd_idx = find_env_var(tools->env, "PWD");
	oldpwd_idx = find_env_var(tools->env, "OLDPWD");
	
	if (pwd_idx == -1 || oldpwd_idx == -1 || !getcwd(current_pwd, PATH_MAX))
		return ;

	if (!tools->env[pwd_idx])
		return ;

	new_oldpwd = ft_strjoin("OLDPWD=", tools->env[pwd_idx] + 4);
	new_pwd = ft_strjoin("PWD=", current_pwd);

	if (!new_oldpwd || !new_pwd)
	{
		if (new_oldpwd)
			free(new_oldpwd);
		if (new_pwd)
			free(new_pwd);
		return ;
	}

	if (tools->env[oldpwd_idx])
		free(tools->env[oldpwd_idx]);
	if (tools->env[pwd_idx])
		free(tools->env[pwd_idx]);

	tools->env[oldpwd_idx] = new_oldpwd;
	tools->env[pwd_idx] = new_pwd;
}

int	builtin_cd(t_tools *tools, char **args)
{
	char	*path;
	char	*expanded_path;
	int		ret;
	int		oldpwd_idx;

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
			return (ft_printf("minishell: cd: HOME not set\n"), 1);
		expanded_path = ft_strdup(path);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		oldpwd_idx = find_env_var(tools->env, "OLDPWD");
		if (oldpwd_idx == -1 || !tools->env[oldpwd_idx])
			return (ft_printf("minishell: cd: OLDPWD not set\n"), 1);
		ft_printf("%s\n", tools->env[oldpwd_idx] + 7);
		expanded_path = ft_strdup(tools->env[oldpwd_idx] + 7);
	}
	else
		expanded_path = expand_home_path(args[1]);

	if (!expanded_path)
		return (1);

	ret = chdir(expanded_path);
	free(expanded_path);

	if (ret == -1)
		return (ft_printf("minishell: cd: %s: %s\n", args[1], strerror(errno)), 1);

	update_pwd_vars(tools);
	return (0);
}
