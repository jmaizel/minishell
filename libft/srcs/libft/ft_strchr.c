/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:03:39 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:37 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	copy_c;

	i = 0;
	copy_c = (char)c;
	while (str[i])
	{
		if (str[i] == copy_c)
			return ((char *) &str[i]);
		i++;
	}
	if (str[i] == copy_c)
		return ((char *) &str[i]);
	return (NULL);
}

/*
#include <stdio.h>

int	main(void)
{
	const char	str[] = "Hello, World!";
	char		*result;

	result = ft_strchr(str, 'W');
	if (result)
		printf("Caractère 'W' trouvé à la position : %ld\n", result - str);
	else
		printf("Caractère 'W' non trouvé.\n");

	result = ft_strchr(str, 'o');
	if (result)
		printf("Caractère 'o' trouvé à la position : %ld\n", result - str);
	else
		printf("Caractère 'o' non trouvé.\n");

	result = ft_strchr(str, 'z');
	if (result)
		printf("Caractère 'z' trouvé à la position : %ld\n", result - str);
	else
		printf("Caractère 'z' non trouvé.\n");
		
	result = ft_strchr(str, '\0');
	if (result)
		printf("Caractère nul '\\0' trouvé à la fin de la chaîne.\n");

	return (0);
}
*/