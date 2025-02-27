/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:34:38 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:32:19 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	compteur;

	i = 0;
	if (needle[0] == '\0')
		return ((char *) haystack);
	while (haystack[i] != '\0' && i < len)
	{
		compteur = 0;
		while (haystack[i + compteur] == needle[compteur]
			&& needle[compteur] != '\0' && i + compteur < len)
		{
			compteur++;
		}
		if (needle[compteur] == '\0')
			return ((char *)haystack + i);
		i++;
	}
	return (0);
}

/*
#include <stdio.h>

int	main(void)
{
	const char	*haystack = "Hello, World!";
	const char	*needle = "World";
	const char	*needle2 = "Test";
	char		*result;

	result = ft_strnstr(haystack, needle, 13);
	if (result)
		printf("'%s' trouvé dans '%s' à la position : %ld\n", needle, haystack, 
			result - haystack);
	else
		printf("'%s' non trouvé dans '%s'\n", needle, haystack);

	result = ft_strnstr(haystack, needle2, 13); 
	if (result)
		printf("'%s' trouvé dans '%s' à la position : %ld\n", needle2, haystack, 
			result - haystack);
	else
		printf("'%s' non trouvé dans '%s'\n", needle2, haystack);

	result = ft_strnstr(haystack, "", 13); 
	if (result)
		printf("Chaîne vide, renvoie : '%s'\n", result);
	else
		printf("Erreur avec une chaîne vide\n");

	result = ft_strnstr(haystack, needle, 5);
	if (result)
		printf("'%s' trouvé dans '%s' à la position : %ld\n", needle, haystack, 
			result - haystack);
	else
		printf("'%s' non trouvé dans les 5 premiers caractères de '%s'\n", 
			needle, haystack);
	return (0);
}
*/