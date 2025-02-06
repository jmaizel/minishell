/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:01:26 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/06 21:36:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/expansion.h"

int builtin_cd(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
{
	(void)tools; // si non utilisé
	char **args;
	char *expanded;
	char *oldpwd = NULL;
	char *newpwd = NULL;
	char *tmp_str = NULL;
	int  ret = BUILTIN_SUCCESS;

	// Expanser la commande brute avant split.
	expanded = expand_variables(cmd->full_cmd, env_mgr);
	if (!expanded)
		return (BUILTIN_ERROR);

	args = ft_split(expanded, ' ');
	free(expanded);
	if (!args)
		return (BUILTIN_ERROR);

	// On récupère le PWD avant de faire chdir
	oldpwd = getcwd(NULL, 0);

	// cd sans argument => $HOME
	if (!args[1])
	{
		char *home = get_env_var("HOME", env_mgr);
		if (!home)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			free_str_array(args);
			free(oldpwd);
			return (BUILTIN_ERROR);
		}
		if (chdir(home) == -1)
		{
			ft_putstr_fd("cd: ", STDERR_FILENO);
			ft_putstr_fd(home, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			ret = BUILTIN_ERROR;
		}
	}
	else
	{
		// cd <chemin>
		if (chdir(args[1]) == -1)
		{
			ft_putstr_fd("cd: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			ret = BUILTIN_ERROR;
		}
	}

	// Mise à jour OLDPWD / PWD si chdir OK
	if (ret == BUILTIN_SUCCESS)
	{
		if (oldpwd)
		{
			tmp_str = ft_strjoin("OLDPWD=", oldpwd);
			if (tmp_str)
			{
				add_env_var(tmp_str, env_mgr);
				free(tmp_str);
			}
		}
		newpwd = getcwd(NULL, 0);
		if (newpwd)
		{
			tmp_str = ft_strjoin("PWD=", newpwd);
			if (tmp_str)
			{
				add_env_var(tmp_str, env_mgr);
				free(tmp_str);
			}
			free(newpwd);
		}
	}

	free_str_array(args);
	free(oldpwd);
	return (ret);
}