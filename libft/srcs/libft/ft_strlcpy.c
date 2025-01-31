/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:33:19 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:01 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t dest_size)
{
	size_t	src_size;

	src_size = ft_strlen(src);
	if (src_size + 1 < dest_size)
		ft_memcpy(dest, src, src_size + 1);
	else if (dest_size != 0)
	{
		ft_memcpy(dest, src, dest_size - 1);
		dest[dest_size - 1] = '\0';
	}
	return (src_size);
}
/*
#include <string.h>
#include <stdio.h>

int main(void)
{
	char *src = "test bonjour 19";
	char dest [20];
	//ft_strlcpy(dest, src, 16);
	strlcpy(dest, src, 15);
	printf("Dest -> %s", dest);
	return (0);
}
*/