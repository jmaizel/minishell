/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:53:19 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:28:36 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_char(va_list args)
{
	char	c;

	c = (char)va_arg(args, int);
	if (write(1, &c, 1) == -1)
		return (-1);
	return (1);
}
