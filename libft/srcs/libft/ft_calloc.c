/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:55:43 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:29:58 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*temp;
	size_t	i;

	if (count == 0 || size == 0)
		return (malloc(0));
	if (size && count > SIZE_MAX / size)
		return (NULL);
	temp = (void *) malloc((count * size) * sizeof(void));
	if (!temp)
		return (NULL);
	i = count * size;
	ft_bzero(temp, i);
	return (temp);
}

/*
#include <stdio.h>

int	main(void)
{
	int	*arr;
	int	i;

	arr = (int *)ft_calloc(5, sizeof(int));
	if (!arr)
	{
		printf("Allocation failed!\n");
		return (1);
	}
	i = 0;
	while (i < 5)
	{
		printf("arr[%d] = %d\n", i, arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
*/