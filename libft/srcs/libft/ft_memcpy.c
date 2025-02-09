/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:52:40 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:03 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tab_dest;
	unsigned char	*tab_src;

	if (dest == (void *)0 && src == (void *)0)
		return (dest);
	tab_dest = (unsigned char *)dest;
	tab_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		tab_dest[i] = tab_src[i];
		i++;
	}
	return (dest);
}
/*
#include <string.h>
#include <stdio.h>

int main(void)
{
	char src[20] = "Bonjour 19";
	char dest[20];
	
	//ft_memcpy(dest, src, 9);
	memcpy(dest, src, 9);
	printf("Src : %s\n", src);
	printf("Dest : %s\n", dest);
	return 0;
}
*/