/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:21:45 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:31:45 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
void	toupper_V3(unsigned int i, char *c) 
{
	(void)i;
	*c = ft_toupper(*c);
}

int main(void)
{
	char str[] = "hello";
	ft_striteri(str, toupper_V3);
	printf("%s\n", str);
}
*/