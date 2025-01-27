/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:43:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 15:38:53 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

char    *get_env_var(char *key, char **env)
{
    int     len;
    int     i;

    if (!key || !env)
        return (NULL);

    len = ft_strlen(key);
    i = 0;
    while (env[i])
    {
        // Vérifie si la variable commence par la clé et est suivie d'un '='
        if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
        {
            // Retourne le pointeur après le '='
            return (env[i] + len + 1);
        }
        i++;
    }
    return (NULL);
}

int     add_env_var(char *var, char ***env)
{
        if (!var || !env || !*env)
                return (GENERAL_ERROR);

        int     i;
        char    **new_env;

        i = 0;
        while ((*env)[i])
        {
                if (ft_strncmp((*env)[i], var, ft_strchr(var, '=') - var) == 0)
                {
                        free((*env)[i]);
                        if (!((*env)[i] = ft_strdup(var)))
                                return (GENERAL_ERROR);
                        return (SUCCESS);
                }
                i++;
        }

        if (!(new_env = malloc(sizeof(char *) * (i + 2))))
                return (GENERAL_ERROR);

        ft_memcpy(new_env, *env, sizeof(char *) * i);
        if (!(new_env[i] = ft_strdup(var)))
        {
                free(new_env);
                return (GENERAL_ERROR);
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
