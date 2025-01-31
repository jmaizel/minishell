/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:54:31 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:11 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*res;
	unsigned int	size;

	if (!s || !f)
		return (NULL);
	size = ft_strlen(s);
	res = (char *)malloc((size + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
/*
char	toupper_V2(unsigned int i, char c)
{
	(void)i;
	return (ft_toupper(c));
}

#include <stdio.h>

int	main(void)
{
	char *str = "hello world!";
	char *res = ft_strmapi(str, toupper_V2);
	printf("str : %s\n", str);
	printf("res : %s\n", res);
	free(res);
	return (0);
}
*/