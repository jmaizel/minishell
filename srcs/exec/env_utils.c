/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:43:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 12:50:26 by cdedessu         ###   ########.fr       */
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
    int     i;
    char    **new_env;

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

    // Allocation d'un nouvel environnement
    new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return (1);

    // Copier l'ancien environnement
    ft_memcpy(new_env, *env, sizeof(char *) * i);
    
    // Ajouter la nouvelle variable
    new_env[i] = ft_strdup(var);
    new_env[i + 1] = NULL;

    // Libérer l'ancien environnement
    free(*env);
    *env = new_env;

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
