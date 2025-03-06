/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:51:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 13:42:41 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
#include "../../includes/builtins.h"

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
