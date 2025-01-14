/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:33:10 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:29 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	count_trim(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*res_creation(char const *s1, int start, int size)
{
	char	*res;
	int		i;

	res = (char *)malloc((size + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = s1[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		count_start;
	int		count_end;

	if (s1 == NULL || set == NULL)
		return (NULL);
	if (ft_strlen(s1) == 0)
	{
		res = ft_strdup("");
		return (res);
	}
	count_start = 0;
	while (count_trim(set, s1[count_start]))
		count_start++;
	count_end = ft_strlen(s1);
	while (count_trim(set, s1[count_end - 1]))
		count_end--;
	if (count_end <= count_start)
		return (ft_strdup(""));
	return (res_creation(s1, count_start, (count_end - count_start)));
}

/*

#include <stdio.h>

int main(void)
{
    char *s1 = "  test  ";
    char *set = " ";
    char *res = ft_strtrim(s1, set);

    if *res)
    {
        printf("Résultat : '%s'\n", res); 
        free(res);
    }
    return (0);
}
*/