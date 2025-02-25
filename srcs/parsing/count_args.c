/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:07:09 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/25 13:25:49 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_quote(char c, int *in_quotes, char *quote_type, int *in_word)
{
	if (!(*in_quotes))
	{
		*quote_type = c;
		*in_quotes = 1;
		if (!(*in_word))
		{
			*in_word = 1;
		}
	}
	else if (c == *quote_type)
		*in_quotes = 0;
}

static void	handle_word(char c, int *count, int *in_word, int in_quotes)
{
	if (!in_quotes && ft_isspace(c))
		*in_word = 0;
	else if (!(*in_word))
	{
		(*count)++;
		*in_word = 1;
	}
}

int	count_args(char *str)
{
	int		count;
	int		i;
	int		in_quotes;
	char	quote_type;
	int		in_word;

	count = 0;
	i = 0;
	in_quotes = 0;
	in_word = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
			handle_quote(str[i], &in_quotes, &quote_type, &in_word);
		else
			handle_word(str[i], &count, &in_word, in_quotes);
		i++;
	}
	return (count);
}