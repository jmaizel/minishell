/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/27 20:54:47 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_consecutive_chars(const char *input, int i)
{
	char	c;
	int		count;

	c = input[i];
	count = 1;
	while (input[i + 1] == c)
	{
		count++;
		i++;
	}
	return (count);
}

static int	handle_redir_error(char *input, int i)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putchar_fd(input[i], 2);
	ft_putstr_fd("'\n", 2);
	return (0);
}

static int	skip_spaces_and_check_end(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (!input[i] || input[i] == '<' || input[i] == '>')
	{
		return (0);
	}
	return (i);
}

int	check_redir_syntax(char *input)
{
	int	i;
	int	count;

	i = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			count = count_consecutive_chars(input, i);
			if (count > 2)
				return (handle_redir_error(input, i));
			i = skip_spaces_and_check_end(input, i + count);
			if (!i)
				return (0);
			i--;
		}
		i++;
	}
	return (1);
}
