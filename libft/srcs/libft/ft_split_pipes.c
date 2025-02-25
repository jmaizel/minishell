/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:44:59 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 17:42:20 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static void	free_str(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void	skip_quotes(const char *str, int *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] == quote)
		(*i)++;
}

static int	count_segments(const char *s, char delimiter)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == delimiter)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != delimiter)
			{
				if (is_quote(s[i]))
					skip_quotes(s, &i);
				else
					i++;
			}
		}
	}
	return (count);
}

static char	*get_next_segment(const char *s, char delimiter, int *index)
{
	int		start;
	int		i;
	int		in_quotes;
	char	quote_type;

	while (s[*index] && s[*index] == delimiter)
		(*index)++;
	start = *index;
	i = *index;
	in_quotes = 0;
	while (s[i] && (in_quotes || s[i] != delimiter))
	{
		if (!in_quotes && (s[i] == '"' || s[i] == '\''))
		{
			quote_type = s[i];
			in_quotes = 1;
		}
		else if (in_quotes && s[i] == quote_type)
			in_quotes = 0;
		i++;
	}
	*index = i;
	return (ft_substr(s, start, i - start));
}

char	**ft_split_pipes(const char *s, char delimiter)
{
	char	**result;
	int		segments;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	segments = count_segments(s, delimiter);
	result = malloc(sizeof(char *) * (segments + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (j < segments)
	{
		result[j] = get_next_segment(s, delimiter, &i);
		if (!result[j])
		{
			free_str(result);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}
