/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:58:31 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:58 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

unsigned int	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	dest_size;
	unsigned int	src_size;
	unsigned int	i;

	if (!dest && !src)
		return (0);
	dest_size = 0;
	while (dest[dest_size] != '\0' && dest_size < size)
		dest_size++;
	src_size = 0;
	while (src[src_size] != '\0')
		src_size++;
	if (size <= dest_size)
		return (size + src_size);
	i = 0;
	while (src[i] != '\0' && dest_size + i < size - 1)
	{
		dest[dest_size + i] = src[i];
		i++;
	}
	dest[dest_size + i] = '\0';
	return (dest_size + src_size);
}

/*

#include <stdio.h>

int	main(void)
{
	char tab1 [] = "Test";
	char tab2 [20] = "Test";
	int res = ft_strlcat(tab2, tab1, 6);

	printf("Destination : %s \n", tab2);
	printf("Taille totale : %d \n", res);

	return 0;
}
*/
