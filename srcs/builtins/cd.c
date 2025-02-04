/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:10:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 21:09:03 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static char	*get_home_directory(t_env_manager *env_mgr)
{
	char	*home;

	if (!env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return (NULL);
	home = get_env_var("HOME", env_mgr);
	if (!home)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (ft_strdup(home));
}

static int	update_pwd_vars(t_env_manager *env_mgr, char *old_pwd)
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
	ret = add_env_var(oldpwd_env, env_mgr);
	if (ret == SUCCESS)
		ret = add_env_var(pwd_env, env_mgr);
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

int	builtin_cd(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
{
	char	*path;
	char	*old_pwd;
	char	**args;

	if (!cmd || !tools || !tools->env)
	{
		ft_putendl_fd("cd: invalid command structure", STDERR_FILENO);
		return (ERR_INVALID_CMD);
	}
	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (ERR_MALLOC_FAILURE);
	if (!args[1])
	{
		path = get_home_directory(env_mgr);
		if (!path)
		{
			free_str_array(args);
			return (ERR_INVALID_CMD);
		}
	}
	else
	{
		path = ft_strdup(args[1]);
		free_str_array(args);
		if (!path)
		{
			ft_putendl_fd("cd: memory allocation failed", STDERR_FILENO);
			return (ERR_MALLOC_FAILURE);
		}
	}
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
	return (update_pwd_vars(env_mgr, old_pwd));
}
