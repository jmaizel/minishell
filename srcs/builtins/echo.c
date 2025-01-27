/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 11:43:17 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	builtin_echo(t_simple_cmds *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->str[1] && ft_strcmp(cmd->str[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->str[i])
	{
		ft_putstr_fd(cmd->str[i], STDOUT_FILENO);
		if (cmd->str[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
