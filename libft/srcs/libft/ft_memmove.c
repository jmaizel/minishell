/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 09:58:02 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:08 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*temp_dest;
	unsigned char	*temp_src;

	if (!dest && !src)
		return (NULL);
	temp_dest = (unsigned char *)dest;
	temp_src = (unsigned char *)src;
	if (temp_dest > temp_src)
	{
		while (n-- > 0)
			temp_dest[n] = temp_src[n];
	}
	else
		ft_memcpy(temp_dest, temp_src, n);
	return (dest);
}

/*
#include <string.h>
#include <stdio.h>

int main(void)
{
	char tab[20] = "Bonjour 19";
	
	void *src = (void *)tab;
	void *dest = (void *)tab + 1;
	
	ft_memmove(dest, src, 9);
	//memmove(dest, src, 9);
	printf("Src : %s\n", src);
	printf("Dest : %s\n", dest);
	return 0;
}
*/