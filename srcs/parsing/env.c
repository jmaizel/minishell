/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:06:11 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/20 11:24:09 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_vars(t_tools *tools)
{
	char	**current_env;

	current_env = tools->env;
	while (*current_env)
	{
		ft_printf("%s\n", *current_env);
		current_env++;
	}
}

char	**get_env_paths(char **env, char *var_name)
{
	int		i;
	char	*value;
	char	**paths;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
		{
			value = env[i] + ft_strlen(var_name) + 1;
			paths = ft_split(value, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}
