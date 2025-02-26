/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_variable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:47:02 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:26:02 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static char	*convert_exit_status(int exit_code)
{
	char	temp[12];
	int		i;

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
	return (ft_strdup(temp));
}

static char	*extract_var_name(const char *str, size_t *i)
{
	size_t	start;
	char	*var_name;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	return (var_name);
}

char	*handle_variable(const char *str, size_t *i, t_tools *tools)
{
	char	*var_name;
	char	*value;

	if (str[*i] == '?')
	{
		(*i)++;
		return (convert_exit_status(tools->exit_code));
	}
	var_name = extract_var_name(str, i);
	if (!var_name)
		return (NULL);
	value = get_var_value(var_name, tools->env);
	free(var_name);
	return (value);
}
