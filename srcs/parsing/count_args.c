/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:07:09 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/03 12:50:47 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(char *str)
{
	int count;
	int i;
	int in_quotes;
	char quote_type;
	int in_word;

	count = 0;
	i = 0;
	in_quotes = 0;
	in_word = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			quote_type = str[i];
			in_quotes = 1;
			if (!in_word)
				count++;
			in_word = 1;
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		else if (!in_quotes && ft_isspace(str[i]))
			in_word = 0;
		else if (!in_word)
		{
			count++;
			in_word = 1;
		}
		i++;
	}
	return (count);
}

