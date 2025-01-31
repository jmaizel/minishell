/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:44:59 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 16:04:32 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/* static void	skip_quotes(const char *str, int *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] == quote)
		(*i)++;
} */

static int	count_segments(const char *s, char delimiter)
{
	int	i;
	int	count;
	int	in_quote;

	i = 0;
	count = 0;
	in_quote = 0;
	while (s[i])
	{
		while (s[i] && s[i] == delimiter && !in_quote)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && (s[i] != delimiter || in_quote))
			{
				if (is_quote(s[i]))
					in_quote = !in_quote; // Toggle quote status
				i++;
			}
		}
	}
	return (count);
}

static char	*get_next_segment(const char *s, char delimiter, int *index)
{
	int	start;
	int	len;
	int	in_quote;

	start = *index;
	len = 0;
	in_quote = 0;
	while (s[*index] && (s[*index] != delimiter || in_quote))
	{
		if (is_quote(s[*index]))
			in_quote = !in_quote;
		(*index)++;
		len++;
	}
	return (ft_substr(s, start, len));
}

char	**ft_split_pipes(const char *s, char delimiter)
{
	char	**result;
	int		segments;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	segments = count_segments(s, delimiter);
	result = malloc(sizeof(char *) * (segments + 1));
	if (!result)
		return (NULL);
	while (j < segments)
	{
		while (s[i] && s[i] == delimiter)
			i++;
		if (s[i])
			result[j++] = get_next_segment(s, delimiter, &i);
	}
	result[j] = NULL;
	return (result);
}

