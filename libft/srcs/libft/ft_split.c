/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 17:35:44 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:34 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (count);
}

static char	*ft_strndup(char const *start, char const *end)
{
	char	*word;
	int		size;
	int		i;

	size = end - start;
	word = (char *)malloc((size + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < end)
	{
		word[i] = *start;
		start++;
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	free_tab(char **tab, int word_index)
{
	int	i;

	i = 0;
	while (i < word_index)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (0);
}

static int	ft_fill_tab(char **tab, char const *s, char c)
{
	int			i;
	int			word_index;
	char const	*start_of_the_word;

	i = 0;
	word_index = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			start_of_the_word = &s[i];
			while (s[i] && s[i] != c)
				i++;
			tab[word_index] = ft_strndup(start_of_the_word, &s[i]);
			if (!tab[word_index])
				return (free_tab(tab, word_index));
			word_index++;
		}
		else
			i++;
	}
	tab[word_index] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	tab = (char **) malloc((word_count + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	if (!ft_fill_tab(tab, s, c))
		return (NULL);
	return (tab);
}

/*
int	main(void)
{
	int		i;
	char	**result;
	char	str[] = "DUCATTI";
	char	charset = 'A';
	
	result = ft_split(str, charset);
	i = 0;
	while (result[i] != NULL)
	{
		printf("%s\n", result[i]);
		i++;
	}
	i = 0;
	while (result[i] != NULL)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (0);
}
*/