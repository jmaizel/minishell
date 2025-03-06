/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:31:05 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/05 13:58:20 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

char	*expand_home_path(char *path)
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

int	update_env_var(t_tools *tools, const char *name, const char *value)
{
	char	*new_var;
	int		idx;

	new_var = ft_strjoin_three(name, "=", value);
	if (!new_var)
		return (0);
	idx = find_env_var(tools->env, name);
	if (idx != -1)
	{
		free(tools->env[idx]);
		tools->env[idx] = new_var;
	}
	else
	{
		tools->env = update_env(tools->env, name, value);
		if (!tools->env)
		{
			free(new_var);
			return (0);
		}
	}
	return (1);
}
