/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:51:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/10 19:09:36 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int	count_env_vars(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

int	find_env_var(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	*create_env_string(const char *name, const char *value)
{
	char	*new_var;
	size_t	name_len;
	size_t	value_len;
	size_t	total_len;

	if (!name || !value)
		return (NULL);
	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	total_len = name_len + value_len + 2;

	new_var = malloc(total_len);
	if (!new_var)
		return (NULL);

	ft_strlcpy(new_var, name, name_len + 1);
	new_var[name_len] = '=';
	ft_strlcpy(new_var + name_len + 1, value, value_len + 1);

	return (new_var);
}

char	**update_env(char **env, const char *name, const char *value)
{
	int		index;
	int		count;
	char	*new_var;
	char	**new_env;

	if (!env || !name || !value)
		return (env);

	index = find_env_var(env, name);
	if (index >= 0)
	{
		new_var = create_env_string(name, value);
		if (!new_var)
			return (env);
		free(env[index]);
		env[index] = new_var;
		return (env);
	}

	count = count_env_vars(env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (env);

	ft_memcpy(new_env, env, sizeof(char *) * count);
	new_var = create_env_string(name, value);
	if (!new_var)
	{
		free(new_env);
		return (env);
	}

	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(env);
	return (new_env);
}

char	*get_env_name(const char *var)
{
	char	*eq_pos;

	if (!var)
		return (NULL);
	eq_pos = ft_strchr(var, '=');
	if (!eq_pos)
		return (ft_strdup(var));
	return (ft_substr(var, 0, eq_pos - var));
}

char	*get_env_value(const char *var)
{
	char	*eq_pos;

	if (!var)
		return (NULL);
	eq_pos = ft_strchr(var, '=');
	if (!eq_pos)
		return (NULL);
	return (ft_strdup(eq_pos + 1));
}
