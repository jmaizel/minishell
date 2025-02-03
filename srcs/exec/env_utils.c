/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:15:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 20:47:16 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

char	*get_env_var(const char *key, t_env_manager *env_mgr)
{
	int	len;
	int	i;

	if (!key || !env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env_mgr->tools->env[i])
	{
		if (ft_strncmp(env_mgr->tools->env[i], key, len) == 0
			&& env_mgr->tools->env[i][len] == '=')
			return (env_mgr->tools->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	add_env_var(char *var, t_env_manager *env_mgr)
{
	int		i;
	char	**new_env;

	if (!var || !env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return (ERR_INVALID_CMD);
	i = 0;
	while (env_mgr->tools->env[i])
	{
		if (ft_strncmp(env_mgr->tools->env[i], var,
				ft_strchr(var, '=') - var) == 0)
		{
			free(env_mgr->tools->env[i]);
			env_mgr->tools->env[i] = ft_strdup(var);
			return (env_mgr->tools->env[i] ? SUCCESS : ERR_MALLOC_FAILURE);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (ERR_MALLOC_FAILURE);
	ft_memcpy(new_env, env_mgr->tools->env, sizeof(char *) * i);
	new_env[i] = ft_strdup(var);
	if (!new_env[i])
		return (free(new_env), ERR_MALLOC_FAILURE);
	new_env[i + 1] = NULL;
	free(env_mgr->tools->env);
	env_mgr->tools->env = new_env;
	return (SUCCESS);
}

int	remove_env_var(char *key, t_env_manager *env_mgr)
{
	int	len;
	int	i;

	if (!key || !env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return (ERR_INVALID_CMD);
	len = ft_strlen(key);
	i = 0;
	while (env_mgr->tools->env[i])
	{
		if (ft_strncmp(env_mgr->tools->env[i], key, len) == 0
			&& env_mgr->tools->env[i][len] == '=')
		{
			free(env_mgr->tools->env[i]);
			while (env_mgr->tools->env[i])
			{
				env_mgr->tools->env[i] = env_mgr->tools->env[i + 1];
				i++;
			}
			return (SUCCESS);
		}
		i++;
	}
	return (ERR_INVALID_CMD);
}

char	**duplicate_env(t_env_manager *env_mgr)
{
	int		i;
	char	**new_env;

	if (!env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return (NULL);
	i = 0;
	while (env_mgr->tools->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env_mgr->tools->env[i])
	{
		new_env[i] = ft_strdup(env_mgr->tools->env[i]);
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

void	free_env(t_env_manager *env_mgr)
{
	int	i;

	if (!env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return ;
	i = 0;
	while (env_mgr->tools->env[i])
	{
		free(env_mgr->tools->env[i]);
		i++;
	}
	free(env_mgr->tools->env);
	env_mgr->tools->env = NULL;
}
