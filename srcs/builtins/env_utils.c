/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:51:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/26 08:41:29 by cdedessu         ###   ########.fr       */
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
	size_t	val_len;
	size_t	total_len;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	if (!value)
	{
		new_var = malloc(name_len + 2);
		if (!new_var)
			return (NULL);
		ft_strlcpy(new_var, name, name_len + 1);
		new_var[name_len] = '=';
		new_var[name_len + 1] = '\0';
		return (new_var);
	}
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

static char	**replace_env_var(char **env, int index, const char *name, const char *value)
{
	char	*new_var;

	new_var = create_env_string(name, value);
	if (!new_var)
		return (env);
	free(env[index]);
	env[index] = new_var;
	return (env);
}

static char	**append_env_var(char **env, const char *name, const char *value)
{
	char	**new_env;
	char	*new_var;
	int		count;
	int		i;

	count = count_env_vars(env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (env);
	i = 0;
	while (i < count)
	{
		new_env[i] = env[i];
		i++;
	}
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

char	**update_env(char **env, const char *name, const char *value)
{
	int	index;

	if (!env || !name)
		return (env);
	index = find_env_var(env, name);
	if (index >= 0)
		return (replace_env_var(env, index, name, value));
	return (append_env_var(env, name, value));
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

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}
