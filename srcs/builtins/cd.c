/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:34 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/02 19:34:45 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static char	*get_home_directory(t_tools *tools)
{
	char	*home;

	if (!tools || !tools->env)
		return (NULL);
	home = get_env_var("HOME", tools->env);
	if (!home)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (ft_strdup(home));
}

static int	update_pwd_vars(t_tools *tools, char *old_pwd)
{
	char	*new_pwd;
	char	*oldpwd_env;
	char	*pwd_env;
	int		ret;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		perror("cd: getcwd failed");
		return (ERR_GETCWD_FAILED);
	}
	oldpwd_env = ft_strjoin("OLDPWD=", old_pwd);
	pwd_env = ft_strjoin("PWD=", new_pwd);
	if (!oldpwd_env || !pwd_env)
	{
		free(old_pwd);
		free(new_pwd);
		free(oldpwd_env);
		free(pwd_env);
		ft_putendl_fd("cd: memory allocation failed", STDERR_FILENO);
		return (ERR_MALLOC_FAILURE);
	}
	ret = add_env_var(oldpwd_env, &tools->env);
	if (ret == SUCCESS)
		ret = add_env_var(pwd_env, &tools->env);
	free(old_pwd);
	free(new_pwd);
	free(oldpwd_env);
	free(pwd_env);
	if (ret != SUCCESS)
	{
		ft_putendl_fd("cd: failed to update environment variables",
			STDERR_FILENO);
		return (ERR_MALLOC_FAILURE);
	}
	return (SUCCESS);
}

static char	*get_target_path(t_parsed_cmd *cmd, t_tools *tools)
{
	char	*path;
	char	**args;

	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (NULL);
	if (!args[1])
		path = get_home_directory(tools);
	else
		path = ft_strdup(args[1]);
	free_str_array(args);
	if (!path)
		ft_putendl_fd("cd: memory allocation failed", STDERR_FILENO);
	return (path);
}

int	builtin_cd(t_parsed_cmd *cmd, t_tools *tools)
{
	char	*path;
	char	*old_pwd;

	if (!cmd || !tools || !tools->env)
	{
		ft_putendl_fd("cd: invalid command structure", STDERR_FILENO);
		return (ERR_INVALID_CMD);
	}
	path = get_target_path(cmd, tools);
	if (!path)
		return (ERR_INVALID_CMD);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		free(path);
		perror("cd: getcwd failed");
		return (ERR_GETCWD_FAILED);
	}
	if (chdir(path) == -1)
	{
		free(path);
		free(old_pwd);
		perror("cd");
		return (ERR_CHDIR_FAILED);
	}
	free(path);
	return (update_pwd_vars(tools, old_pwd));
}
