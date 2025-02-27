/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:31 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:29:54 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
/*
#include <stdio.h>

int main(void)
{
	char str[20] = "Bonjour 19";
	printf("Avant : %s\n", str);
	ft_bzero(str, 5);
	//bzero(str, 5);
	printf("Apres : %s\n", str);
	return 0;
}
*/