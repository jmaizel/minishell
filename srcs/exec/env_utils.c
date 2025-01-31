/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 11:43:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 15:21:37 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

char	*get_env_var(const char *key, char **env)
{
	int	len;
	int	i;

	if (!key || !env)
		return (NULL);
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
	int			i;
	char		**new_env;

	if (!var || !env || !*env)
		return (ERR_INVALID_CMD);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], var, ft_strchr(var, '=') - var) == 0)
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			if (!(*env)[i])
				return (ERR_MALLOC_FAILURE);
			return (SUCCESS);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (ERR_MALLOC_FAILURE);
	ft_memcpy(new_env, *env, sizeof(char *) * i);
	new_env[i] = ft_strdup(var);
	if (!new_env[i])
	{
		free(new_env);
		return (ERR_MALLOC_FAILURE);
	}
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
	return (SUCCESS);
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

char	**duplicate_env(char **env)
{
	int			i;
	char		**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}
