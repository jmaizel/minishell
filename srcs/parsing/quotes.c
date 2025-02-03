/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:04:34 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/03 12:05:35 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*clean_quotes(char *str)
{
	char *result;
	int i;
	int j;
	int in_quotes;
	char quote_type;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			quote_type = str[i];
			in_quotes = 1;
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}