/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:37:58 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/05 13:58:38 by jmaizel          ###   ########.fr       */
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

char	*create_env_string_no_value(const char *name, size_t name_len)
{
	char	*new_var;

	new_var = malloc(name_len + 2);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, name, name_len + 1);
	new_var[name_len] = '=';
	new_var[name_len + 1] = '\0';
	return (new_var);
}

char	*create_env_string(const char *name, const char *value)
{
	char	*new_var;
	size_t	name_len;
	size_t	val_len;
	size_t	total_len;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	if (!value)
		return (create_env_string_no_value(name, name_len));
	val_len = ft_strlen(value);
	total_len = name_len + val_len + 2;
	new_var = malloc(total_len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, name, name_len + 1);
	new_var[name_len] = '=';
	ft_strlcpy(new_var + name_len + 1, value, val_len + 1);
	return (new_var);
}

char	**replace_env_var(char **env, int index, const char *name,
		const char *value)
{
	char	*new_var;

	new_var = create_env_string(name, value);
	if (!new_var)
		return (env);
	free(env[index]);
	env[index] = new_var;
	return (env);
}
