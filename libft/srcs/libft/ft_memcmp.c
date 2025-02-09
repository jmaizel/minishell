/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:20:46 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:00 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	size_t			i;
	unsigned char	*str1_copy;
	unsigned char	*str2_copy;

	i = 0;
	str1_copy = (unsigned char *)str1;
	str2_copy = (unsigned char *)str2;
	while (i < n)
	{
		if (str1_copy[i] != str2_copy[i])
			return (str1_copy[i] - str2_copy[i]);
		i++;
	}
	return (0);
}

/*
#include <stdio.h>

int	main(void)
{
	char	str1[] = "HelloWorld";
	char	str2[] = "HelloWorld";
	char	str3[] = "Hello42";
	int		result;

	result = ft_memcmp(str1, str2, 10);
	printf("Comparaison entre str1 et str2 : %d\n", result);

	result = ft_memcmp(str1, str3, 10);
	printf("Comparaison entre str1 et str3 : %d\n", result);

	result = ft_memcmp(str1, str3, 5);
	printf("Comparaison entre str1 et str3 : %d\n", result);

	return (0);
}
*/