/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:54:23 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 16:32:26 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	execute_cmd(t_pip *cmd, t_exec *exec, char *cmd_path)
{
	t_cmd_args	*args;
	char		*cmd_str;

	setup_child_signals();
	if (cmd->redirection)
	{
		if (setup_redirections(cmd->redirection, &exec->process, exec) == -1)
			exit(1);
		cmd_str = cmd->redirection->cmd;
	}
	else
		cmd_str = cmd->cmd_pipe;
	args = parse_command_args(cmd_str);
	if (!args || !args->argv[0])
	{
		free_cmd_args(args);
		exit(1);
	}
	execve(cmd_path, args->argv, exec->tools->env);
	ft_printf("minishell: %s: command not found\n", args->argv[0]);
	free_cmd_args(args);
	exit(127);
}

char	*get_expanded_command(t_pip *cmd, t_exec *exec)
{
	if (cmd->redirection)
		return (expand_str(cmd->redirection->cmd, exec->tools));
	return (expand_str(cmd->cmd_pipe, exec->tools));
}
