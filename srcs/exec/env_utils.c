/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:43:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 17:43:31 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

char	*get_env_var(char *key, char **env)
{
	int	len;
	int	i;

	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	add_env_var(char *var, char ***env)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], var, ft_strchr(var, '=') - var) == 0)
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			return (0);
		}
		i++;
	}
	*env = ft_realloc(*env, sizeof(char *) * (i + 2));
	(*env)[i] = ft_strdup(var);
	(*env)[i + 1] = NULL;
	return (0);
}

int	remove_env_var(char *key, char ***env)
{
	int	len;
	int	i;

	len = ft_strlen(key);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			while ((*env)[i])
			{
				(*env)[i] = (*env)[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
