/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:37 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/08 17:42:40 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	print_error(char *cmd, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (message)
		ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

int	handle_cmd_error(char *cmd, char *error)
{
	print_error(cmd, NULL, error);
	return (127);
}

int	handle_file_error(char *cmd, char *file, char *error)
{
	print_error(cmd, file, error);
	return (1);
}

int	handle_pipe_error(void)
{
	print_error(NULL, NULL, "pipe error");
	return (1);
}

int	handle_fork_error(void)
{
	print_error(NULL, NULL, "fork error");
	return (1);
}
