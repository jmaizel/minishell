/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:28:43 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:37:25 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_ptr(va_list args)
{
	unsigned long	ptr;
	char			*base;
	int				size;

	ptr = (unsigned long)va_arg(args, void *);
	base = "0123456789abcdef";
	size = 0;
	if (!ptr)
	{
		if (write(1, "0x0", 3) == -1)
			return (-1);
		return (3);
	}
	if (write(1, "0x", 2) == -1)
		return (-1);
	size += 2;
	size += ft_print_hex_bis(ptr, base);
	return (size);
}
