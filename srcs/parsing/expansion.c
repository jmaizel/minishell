/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:47:02 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/15 10:17:59 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_exit_status(int exit_code)
{
	char	*result;
	int		i;

	char temp[12]; // assez grand pour un int
	i = 0;
	if (exit_code == 0)
		temp[i++] = '0';
	else
	{
		while (exit_code > 0)
		{
			temp[i++] = (exit_code % 10) + '0';
			exit_code /= 10;
		}
	}
	temp[i] = '\0';
	result = ft_strdup(temp);
	return (result);
}

char	*get_var_value(const char *var_name, char **env)
{
	int		i;
	size_t	len;

	if (!var_name || !env)
		return (ft_strdup(""));
	len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}
static char	*ft_realloc(char *ptr, size_t old_size, size_t new_size)
{
	char	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size <= old_size)
		return (ptr);
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

char	*expand_str(const char *str, t_tools *tools)
{
	char	*result;
	size_t	result_size;
	size_t	i;
	size_t	j;
	int		in_single_quotes;
	char	*value;
	char	*new_result;
	char	*var_name;
	size_t	var_start;
	size_t	val_len;

	if (!str || !tools || !tools->env)
		return (ft_strdup(""));
	// Préallouer une taille suffisante
	result_size = 2048;
	result = ft_calloc(result_size, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quotes = 0;
	while (str[i])
	{
		// Gestion des guillemets simples
		if (str[i] == '\'')
		{
			in_single_quotes = !in_single_quotes;
			result[j++] = str[i++];
			continue ;
		}
		// Traitement des variables d'environnement
		if (str[i] == '$' && !in_single_quotes)
		{
			i++; // Passer le $
			// Gérer le cas spécial $?
			if (str[i] == '?')
			{
				value = expand_exit_status(tools->exit_code);
				i++;
			}
			// Gérer les variables normales
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				var_start = i;
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
				var_name = ft_substr(str, var_start, i - var_start);
				if (!var_name)
					continue ;
				value = get_var_value(var_name, tools->env);
				free(var_name);
			}
			else
			{
				result[j++] = '$';
				continue ;
			}
			// Copier la valeur dans le résultat
			if (value)
			{
				val_len = ft_strlen(value);
				// Vérifier s'il faut agrandir le buffer
				if (j + val_len >= result_size)
				{
					new_result = ft_realloc(result, result_size, result_size
							+ val_len + 1024);
					if (!new_result)
					{
						free(result);
						free(value);
						return (NULL);
					}
					result = new_result;
					result_size = result_size + val_len + 1024;
				}
				ft_strlcpy(result + j, value, result_size - j);
				j += val_len;
				free(value);
			}
		}
		else
		{
			result[j++] = str[i++];
		}
		// Vérifier s'il faut agrandir le buffer
		if (j >= result_size - 1)
		{
			new_result = ft_realloc(result, result_size, result_size + 1024);
			if (!new_result)
			{
				free(result);
				return (NULL);
			}
			result = new_result;
			result_size += 1024;
		}
	}
	result[j] = '\0';
	return (result);
}
