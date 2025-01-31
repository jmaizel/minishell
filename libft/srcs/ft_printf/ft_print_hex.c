/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 10:58:03 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:28:43 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_hex_bis(unsigned long num, char *base)
{
	int	size;

	size = 0;
	if (num >= 16)
	{
		size += ft_print_hex_bis(num / 16, base);
		if (size == -1)
			return (-1);
	}
	if (write(1, &base[num % 16], 1) == -1)
		return (-1);
	size++;
	return (size);
}

int	ft_print_hex(va_list args, int uppercase)
{
	unsigned long	num;
	char			*base;
	int				size;

	num = va_arg(args, unsigned int);
	base = NULL;
	if (uppercase == 1)
		base = "0123456789ABCDEF";
	else if (uppercase == 0)
		base = "0123456789abcdef";
	size = 0;
	size += ft_print_hex_bis(num, base);
	if (size == -1)
		return (-1);
	return (size);
}
