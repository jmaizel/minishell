/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:25:47 by cdedessu          #+#    #+#             */
/*   Updated: 2024/12/24 15:37:40 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	copy_c;
	unsigned char	*copy_str;

	copy_c = (unsigned char)c;
	copy_str = (unsigned char *)str;
	i = 0;
	while (i < n)
	{
		if (copy_str[i] == copy_c)
			return ((void *)&copy_str[i]);
		i++;
	}
	return (NULL);
}
/*

#include <stdio.h>

int	main(void)
{
	char data[] = {10 , 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int size = 10;

	void * found = ft_memchr( data, 57, size );
    printf( "57 is %s\n", ( found != NULL ? "found" : "not found" ) );

	found = ft_memchr( data, 50, size );
    printf( "50 is %s\n", ( found != NULL ? "found" : "not found" ) );
    if ( found != NULL ) {
        printf( "La valeur à la position calculée est %d\n", *((char *) found) );
    }
	return (0);
}
*/