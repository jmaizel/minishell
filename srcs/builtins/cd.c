/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:04:22 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/03/05 13:31:23 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

static void	update_pwd_vars(t_tools *tools, const char *old_pwd)
{
	char	current_pwd[PATH_MAX];

	if (!getcwd(current_pwd, PATH_MAX))
		return ;
	if (old_pwd)
		update_env_var(tools, "OLDPWD", old_pwd);
	update_env_var(tools, "PWD", current_pwd);
}

static char	*get_home_path(t_tools *tools)
{
	int		home_idx;
	char	*path;

	home_idx = find_env_var(tools->env, "HOME");
	if (home_idx == -1)
	{
		ft_printf("minishell: cd: HOME not set\n");
		return (NULL);
	}
	path = tools->env[home_idx] + 5;
	return (ft_strdup(path));
}

static char	*get_oldpwd_path(t_tools *tools)
{
	int		oldpwd_idx;
	char	*expanded_path;

	oldpwd_idx = find_env_var(tools->env, "OLDPWD");
	if (oldpwd_idx == -1)
	{
		ft_printf("minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	expanded_path = ft_strdup(tools->env[oldpwd_idx] + 7);
	if (expanded_path)
		ft_printf("%s\n", expanded_path);
	return (expanded_path);
}

int	builtin_cd(t_tools *tools, char **args)
{
	char	*expanded_path;
	int		ret;
	char	old_pwd[PATH_MAX];

	expanded_path = NULL;
	if (!getcwd(old_pwd, PATH_MAX))
		return (1);
	if (!args[1])
		expanded_path = get_home_path(tools);
	else if (ft_strcmp(args[1], "-") == 0)
		expanded_path = get_oldpwd_path(tools);
	else
		expanded_path = expand_home_path(args[1]);
	if (!expanded_path)
		return (1);
	ret = chdir(expanded_path);
	if (ret == -1)
	{
		ft_printf("minishell: cd: %s: No such file or directory\n", args[1]);
		free(expanded_path);
		return (1);
	}
	update_pwd_vars(tools, old_pwd);
	free(expanded_path);
	return (0);
}
