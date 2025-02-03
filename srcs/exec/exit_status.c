/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:15:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 20:56:54 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	update_exit_status(t_tools *tools, int status)
{
	if (!tools)
		return ;
	if (WIFEXITED(status))
		tools->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		tools->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		else if (WTERMSIG(status) == SIGKILL)
			ft_putendl_fd("Killed", STDERR_FILENO);
	}
}

int	get_command_exit_status(t_pip *pip)
{
	t_cmd_args	*args;
	int			exit_status;

	if (!pip || !pip->redirection || !pip->redirection->cmd)
		return (GENERAL_ERROR);
	args = parse_command_args(pip->redirection->full_cmd);
	if (!args || !args->argv || !args->argv[0])
	{
		free_cmd_args(args);
		return (GENERAL_ERROR);
	}
	if (ft_strcmp(args->argv[0], "exit") == 0)
	{
		exit_status = (args->argv[1]) ? ft_atoi(args->argv[1]) : 0;
		free_cmd_args(args);
		return (exit_status);
	}
	free_cmd_args(args);
	return (GENERAL_ERROR);
}
