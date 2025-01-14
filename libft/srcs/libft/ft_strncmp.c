/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 12:58:33 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:15 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/*

#include <stdio.h>

int	main(void)
{
	char tab1 [] = "TestTestA";
	// char tab2 [] = "Test";
	char tab3 [] = "TestTest";
	int res = ft_strncmp(tab3, tab1, 20);

	printf("Resultat du strcmp : %d", res);

	return 0;
}
*/