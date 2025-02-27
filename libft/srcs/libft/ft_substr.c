/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:18:02 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:33 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		res = ft_strdup("");
		return (res);
	}
	if (len > (ft_strlen(s) - start))
		len = (ft_strlen(s) - start);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

/*
#include <stdio.h>

int	main(void)
{
	char	*str = "Hello, World!";
	char	*substr;

	substr = ft_substr(str, 7, 5);
	printf("Sous-chaîne : %s\n", substr);
	free(substr);

	substr = ft_substr(str, 0, 5);
	printf("Sous-chaîne : %s\n", substr);
	free(substr);

	substr = ft_substr(str, 20, 5);
	printf("Sous-chaîne : %s\n", substr);
	free(substr);

	return (0);
}
*/