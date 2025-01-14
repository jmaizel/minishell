/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:49:07 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:42 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strdup(const char *src)
{
	int		src_size;
	char	*copy;
	int		i;

	src_size = ft_strlen(src);
	copy = (char *) malloc((src_size + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < src_size)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
/*

#include <stdio.h>

int	main(void)
{
	char *tab = "Bonjour 19";
	char *copy;

	copy = ft_strdup(tab);
	if (copy == NULL)
		printf("Erreur d'allocation");
	else
	{
		printf("Copy : %s\n", copy);
		free(copy);
	}
	return (0);
}
*/