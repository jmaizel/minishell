/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:54:49 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:28:56 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_str(va_list args)
{
	char	*str;
	int		size;

	str = va_arg(args, char *);
	if (!str)
		str = "(null)";
	size = 0;
	while (str[size])
	{
		if (write(1, &str[size], 1) == -1)
			return (-1);
		size++;
	}
	return (size);
}
