/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:22:41 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	setup_output(t_parsed_cmd *cmd, t_process *process)
{
	if (cmd->output_count <= 0)
		return (0);
	process->stdout_backup = dup(STDOUT_FILENO);
	if (process->stdout_backup == -1)
	{
		ft_printf("minishell: dup error\n");
		return (-1);
	}
	if (open_intermediate_files(cmd) == -1)
		return (-1);
	if (!cmd->output_file[cmd->output_count - 1])
	{
		ft_printf("minishell: invalid final output file\n");
		return (-1);
	}
	if (handle_output_redir(cmd->output_file[cmd->output_count - 1], 0) == -1)
		return (-1);
	return (0);
}

int	setup_redirections(t_parsed_cmd *cmd, t_process *process, t_exec *exec)
{
	if (!cmd)
		return (0);
	process->stdin_backup = -1;
	process->stdout_backup = -1;
	if (setup_input(cmd, process, exec) == -1)
		return (-1);
	if (setup_output(cmd, process) == -1)
		return (-1);
	return (0);
}

void	restore_redirections(t_process *process)
{
	if (process->stdin_backup != -1)
	{
		if (dup2(process->stdin_backup, STDIN_FILENO) == -1)
			ft_printf("minishell: error restoring stdin\n");
		close(process->stdin_backup);
		process->stdin_backup = -1;
	}
	if (process->stdout_backup != -1)
	{
		if (dup2(process->stdout_backup, STDOUT_FILENO) == -1)
			ft_printf("minishell: error restoring stdout\n");
		close(process->stdout_backup);
		process->stdout_backup = -1;
	}
}
