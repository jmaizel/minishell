/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:22:59 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 13:21:28 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

/* t_cmd_args	*get_command_args(t_pip *cmd)
{
	if (!cmd->redirection)
		cmd->redirection = parse_redir(cmd->cmd_pipe);
	if (cmd->redirection)
		return (parse_command_args(cmd->redirection->cmd));
	return (parse_command_args(cmd->cmd_pipe));
} */

char	*get_command_path(t_cmd_args *args, t_exec *exec)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(args->argv[0], exec->cmd_paths);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", args->argv[0]);
		free_cmd_args(args);
		exit(127);
	}
	return (cmd_path);
}
