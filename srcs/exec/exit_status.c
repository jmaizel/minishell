/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:07:36 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 13:30:10 by cdedessu         ###   ########.fr       */
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
		if (args->argv[1])
			exit_status = ft_atoi(args->argv[1]);
		else
			exit_status = 0;
		free_cmd_args(args);
		return (exit_status);
	}
	free_cmd_args(args);
	return (GENERAL_ERROR);
}