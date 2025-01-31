/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:27:15 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:12 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*tab;

	tab = (unsigned char *)str;
	i = 0;
	while (i < n)
	{
		tab[i] = (unsigned char)c;
		i++;
	}
	return (str);
}
/*
#include <string.h>
#include <stdio.h>

int	main(void)
{
	char str[50] = "Bonjour 19";
	printf("Avant -> %s\n", str);
	ft_memset(str, 'B', 10);
	//memset(str, 'B', 10);
	printf("Apres -> %s\n", str);
	return (0);
}
*/