/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:44:03 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 17:46:24 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static char	*expand_var(char *str, t_tools *tools)
{
	char	*key;
	char	*value;
	char	*result;

	key = str;
	while (*str && (ft_isalnum(*str) || *str == '_'))
		str++;
	*str = '\0';
	value = get_env_var(key, tools->env);
	if (value != NULL)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	return (result);
}

static char	*append_char_to_str(char *str, char c)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

char	*expand_variables(char *str, t_tools *tools)
{
	char	*expanded;
	char	*temp;
	char	*var;

	expanded = ft_strdup("");
	while (*str)
	{
		if (*str == '$' && ft_isalpha(*(str + 1)))
		{
			str++;
			temp = expanded;
			var = expand_var(str, tools);
			expanded = ft_strjoin(expanded, var);
			free(temp);
			free(var);
			while (ft_isalnum(*str) || *str == '_')
				str++;
		}
		else
		{
			temp = expanded;
			expanded = append_char_to_str(expanded, *str);
			free(temp);
			str++;
		}
	}
	return (expanded);
}
