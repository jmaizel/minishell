/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:47:02 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/13 16:01:26 by jmaizel          ###   ########.fr       */
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
static int	get_var_length(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '?')
		return (1);
	// Premier caractère doit être une lettre ou underscore
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	// Continue tant que c'est un caractère valide
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*get_var_value(const char *var_name, char **env)
{
	int		i;
	size_t	len;

	if (!var_name || !env)
		return (ft_strdup(""));
	len = ft_strlen(var_name);
	if (len == 0)
		return (ft_strdup(""));
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_str(const char *str, t_tools *tools)
{
	char	*result;
	int		i;
	int		j;
	int		in_single_quotes;
	char	*exit_str;
	int		var_len;
	char	*var_name;
	char	*value;

	if (!str || !tools || !tools->env)
		return (ft_strdup(""));
	result = malloc(sizeof(char) * (ft_strlen(str) * 2 + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_single_quotes)
			in_single_quotes = 1;
		else if (str[i] == '\'' && in_single_quotes)
			in_single_quotes = 0;
		else if (str[i] == '$' && !in_single_quotes)
		{
			if (!str[i + 1])
			{
				result[j++] = str[i++];
				continue ;
			}
			i++;
			if (str[i] == '?')
			{
				exit_str = expand_exit_status(tools->exit_code);
				if (exit_str)
				{
					ft_strlcpy(&result[j], exit_str, ft_strlen(exit_str) + 1);
					j += ft_strlen(exit_str);
					free(exit_str);
				}
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				var_len = get_var_length(&str[i]);
				if (var_len > 0)
				{
					var_name = ft_substr(str, i, var_len);
					if (var_name)
					{
						value = get_var_value(var_name, tools->env);
						if (value)
						{
							ft_strlcpy(&result[j], value, ft_strlen(value) + 1);
							j += ft_strlen(value);
							free(value);
						}
						free(var_name);
					}
					i += var_len;
				}
			}
			else
				result[j++] = '$';
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
