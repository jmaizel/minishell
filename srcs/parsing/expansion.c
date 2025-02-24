/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:47:02 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/24 13:13:24 by jacobmaizel      ###   ########.fr       */
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

char	*resize_buffer(char *result, size_t *size, size_t needed_size)
{
	char	*new_result;

	if (needed_size >= *size)
	{
		new_result = ft_realloc(result, *size, *size + needed_size + 1024);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		*size += needed_size + 1024;
		return (new_result);
	}
	return (result);
}

static char	*handle_variable(const char *str, size_t *i, t_tools *tools)
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

static size_t	copy_value(char **result, size_t j, char *value, size_t *size)
{
	size_t	len;

	len = ft_strlen(value);
	*result = resize_buffer(*result, size, j + len);
	if (!*result)
		return (0);
	ft_memcpy(*result + j, value, len);
	free(value);
	return (j + len);
}

static size_t	process_dollar(const char *str, t_expand *exp, char **result,
									t_tools *tools)
{
	char	*value;

	(exp->i)++;
	value = handle_variable(str, &(exp->i), tools);
	if (!value)
		return (free(*result), 0);
	exp->j = copy_value(result, exp->j, value, &(exp->size));
	return (1);
}

static int	handle_char(const char *str, char **result, t_expand *exp,
				t_tools *tools)
{
	if (str[exp->i] == '\'')
		exp->in_quotes = !(exp->in_quotes);
	else if (str[exp->i] == '$' && !(exp->in_quotes))
	{
		if (!process_dollar(str, exp, result, tools))
			return (0);
		return (1);
	}
	else
		(*result)[(exp->j)++] = str[(exp->i)++];
	*result = resize_buffer(*result, &exp->size, exp->j);
	return (1);
}

char	*expand_str(const char *str, t_tools *tools)
{
	char		*result;
	t_expand	exp;

	exp.size = 2048;
	if (!str || !tools || !tools->env)
		return (ft_strdup(""));
	result = ft_calloc(exp.size, sizeof(char));
	if (!result)
		return (NULL);
	exp.i = 0;
	exp.j = 0;
	exp.in_quotes = 0;
	while (str[exp.i])
		if (!handle_char(str, &result, &exp, tools))
			return (NULL);
	result[exp.j] = '\0';
	return (result);
}