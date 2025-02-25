/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:04:34 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/25 14:25:10 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_quotes(char *str, int *i, int *in_quotes, char *quote_type)
{
	if (!*in_quotes && (str[*i] == '"' || str[*i] == '\''))
	{
		*quote_type = str[*i];
		*in_quotes = 1;
	}
	else if (*in_quotes && str[*i] == *quote_type)
		*in_quotes = 0;
}

static char	*allocate_result_buffer(char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	return (result);
}

char	*clean_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		in_quotes;
	char	quote_type;

	result = allocate_result_buffer(str);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quotes = 0;
	while (str[i])
	{
		process_quotes(str, &i, &in_quotes, &quote_type);
		if (!((!in_quotes && (str[i] == '"' || str[i] == '\'')) || (in_quotes
					&& str[i] == quote_type)))
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
