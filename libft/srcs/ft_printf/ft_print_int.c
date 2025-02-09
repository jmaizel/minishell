/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:42:17 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:28:51 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_print_int_bis(int n)
{
	char	c;
	int		size;

	size = 0;
	if (n > 9)
	{
		size += ft_print_int_bis(n / 10);
		if (size == -1)
			return (-1);
	}
	c = (n % 10) + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	size++;
	return (size);
}

int	ft_print_int(va_list args)
{
	int	nbr;
	int	size;

	nbr = va_arg(args, int);
	size = 0;
	if (nbr == -2147483648)
	{
		if (write(1, "-2147483648", 11) == -1)
			return (-1);
		return (11);
	}
	if (nbr < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		nbr = -nbr;
		size++;
	}
	size += ft_print_int_bis(nbr);
	if (size == -1)
		return (-1);
	return (size);
}
