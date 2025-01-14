/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:29 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:24 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		i;
	char	copy_c;
	char	*res;

	res = NULL;
	i = 0;
	copy_c = (char)c;
	while (str[i])
	{
		if (str[i] == copy_c)
			res = ((char *) &str[i]);
		i++;
	}
	if (str[i] == copy_c)
		res = ((char *) &str[i]);
	return (res);
}

/*
#include <stdio.h>

int	main(void)
{
	const char	*str = "Hello, World!";
	char		*result;

	result = ft_strrchr(str, 'o');
	if (result)
		printf("Dernière occurrence de 'o' trouvée à la position : %ld\n", 
			result - str);
	else
		printf("Caractère 'o' non trouvé.\n");

	result = ft_strrchr(str, 'l');
	if (result)
		printf("Dernière occurrence de 'l' trouvée à la position : %ld\n", 
			result - str);
	else
		printf("Caractère 'l' non trouvé.\n");

	result = ft_strrchr(str, 'z');
	if (result)
		printf("Dernière occurrence de 'z' trouvée à la position : %ld\n", 
			result - str);
	else
		printf("Caractère 'z' non trouvé.\n");

	result = ft_strrchr(str, '\0');
	if (result)
		printf("Caractère nul '\\0' trouvé à la position : %ld\n", result - str);
	else
		printf("Caractère nul '\\0' non trouvé.\n");

	return (0);
}
*/