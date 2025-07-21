/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:26:34 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/13 11:27:20 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int	compare_env_vars(const char *s1, const char *s2)
{
	return (ft_strcmp(s1, s2));
}

void	sort_env_vars(char **env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (compare_env_vars(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	copy_and_sort_env(t_tools *tools, char ***sorted_env)
{
	int	env_count;
	int	i;

	env_count = 0;
	while (tools->env && tools->env[env_count])
		env_count++;
	if (env_count == 0)
		return (0);
	*sorted_env = malloc(sizeof(char *) * (env_count + 1));
	if (!*sorted_env)
		return (0);
	i = 0;
	while (i < env_count)
	{
		(*sorted_env)[i] = ft_strdup(tools->env[i]);
		if (!(*sorted_env)[i])
		{
			while (--i >= 0)
				free((*sorted_env)[i]);
			return (free(*sorted_env), 0);
		}
		i++;
	}
	(*sorted_env)[env_count] = NULL;
	return (sort_env_vars(*sorted_env, env_count), env_count);
}
