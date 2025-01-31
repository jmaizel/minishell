/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_int.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 10:31:32 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:29:00 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_print_unsigned_int_bis(unsigned int nbr)
{
	char	c;
	int		size;

	size = 0;
	if (nbr > 9)
	{
		size += ft_print_unsigned_int_bis(nbr / 10);
		if (size == -1)
			return (-1);
	}
	c = (nbr % 10) + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	size++;
	return (size);
}

int	ft_print_unsigned_int(va_list args)
{
	unsigned int	nbr;
	int				size;

	nbr = va_arg(args, unsigned int);
	size = 0;
	size += ft_print_unsigned_int_bis(nbr);
	if (size == -1)
		return (-1);
	return (size);
}
