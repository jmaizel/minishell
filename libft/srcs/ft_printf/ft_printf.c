/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:47:18 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/19 11:52:34 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	print_format(char c, va_list args)
{
	if (c == 'c')
		return (ft_print_char(args));
	else if (c == 's')
		return (ft_print_str(args));
	else if (c == '%')
		return (write(1, "%", 1));
	else if (c == 'p')
		return (ft_print_ptr(args));
	else if (c == 'd' || c == 'i')
		return (ft_print_int(args));
	else if (c == 'u')
		return (ft_print_unsigned_int(args));
	else if (c == 'x')
		return (ft_print_hex(args, 0));
	else if (c == 'X')
		return (ft_print_hex(args, 1));
	return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		size;
	int		temp;

	size = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (!*format)
				return (-1);
			temp = print_format(*format, args);
		}
		else
			temp = write(1, format, 1);
		if (temp == -1)
			return (-1);
		size += temp;
		format++;
	}
	va_end(args);
	return (size);
}

/*
#include <stdio.h>

int	main(void)
{
	ft_printf("ft_printf : %%c : %c\n", 'a');
	printf("printf : %%c : %c\n", 'a');
	ft_printf("ft_printf :%%s : %s\n", "Bonjour 19");
	printf("printf :%%s : %s\n", "Bonjour 19");
	ft_printf("ft_printf : %% : %%\n");
	printf("printf : %% : %%\n");
	ft_printf("ft_printf : %%p : %p\n", (void *)123456789);
	printf("printf : %%p : %p\n", (void *)123456789);
	ft_printf("ft_printf : %%d : %d\n", 17);
	printf("printf : %%d : %d\n", 17);
	ft_printf("ft_printf : %%i : %i\n", 18);
	printf("printf : %%i : %i\n", 18);
	ft_printf("ft_printf : %%u : %u\n", 19);
	printf("printf : %%u : %u\n", 19);
	ft_printf("ft_printf : %%x : %x\n", 42);
	printf("printf : %%x : %x\n", 42);
	ft_printf("ft_printf : %%X : %X\n", 42);
	printf("printf : %%X : %X\n", 42);
	// multiple
	ft_printf("ft_printf : INT : %d  CHAR : %c  HEX MAJ : %X\n", 19, 't', 42);
	printf("printf : INT : %d  CHAR : %c  HEX MAJ : %X\n", 19, 't', 42);
	return (0);
}
*/