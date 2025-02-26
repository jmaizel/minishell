/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:47:02 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:25:33 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	process_dollar(const char *str, t_expand *exp, char **result,
		t_tools *tools)
{
	char	*value;

	(exp->i)++;
	if (ft_isalpha(str[exp->i]) || str[exp->i] == '_' || str[exp->i] == '?')
	{
		value = handle_variable(str, &(exp->i), tools);
		if (!value)
			return (0);
		exp->j = copy_value(result, exp->j, value, &(exp->size));
		if (!*result)
			return (0);
	}
	else
	{
		(*result)[exp->j++] = '$';
	}
	return (1);
}

static int	handle_char(const char *str, char **result, t_expand *exp,
		t_tools *tools)
{
	if (str[exp->i] == '\'')
	{
		(*result)[exp->j++] = str[exp->i++];
		exp->in_quotes = !(exp->in_quotes);
	}
	else if (str[exp->i] == '$' && !(exp->in_quotes))
	{
		if (!process_dollar(str, exp, result, tools))
			return (0);
	}
	else
		(*result)[exp->j++] = str[exp->i++];
	*result = resize_buffer(*result, &exp->size, exp->j);
	if (!*result)
		return (0);
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
	{
		if (!handle_char(str, &result, &exp, tools))
			return (NULL);
	}
	result[exp.j] = '\0';
	return (result);
}
