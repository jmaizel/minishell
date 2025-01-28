/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:07:36 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 16:45:12 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	update_exit_status(t_tools *tools, int status)
{
	if (WIFEXITED(status))
		tools->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		tools->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	}
}

int	get_command_exit_status(t_simple_cmds *cmd)
{
	if (!cmd || !cmd->str || !cmd->str[0])
		return (GENERAL_ERROR);
	if (ft_strcmp(cmd->str[0], "exit") == 0)
	{
		if (cmd->str[1])
			return (ft_atoi(cmd->str[1]));
		return (0);
	}
	return (GENERAL_ERROR);
}
