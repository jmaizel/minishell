/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 20:51:59 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	apply_final_redirection(t_parsed_cmd *cmd, t_process *process)
{
	if (backup_stdout(process) == -1)
		return (-1);
	if (cmd->output_count > 0)
		return (handle_output_redir(cmd->output_file[cmd->output_count - 1],
				0));
	else if (cmd->append_count > 0)
		return (handle_output_redir(cmd->append_file[cmd->append_count - 1],
				1));
	return (0);
}

static int	handle_output_redirs(t_parsed_cmd *cmd, t_process *process)
{
	if (cmd->output_count > 0 || cmd->append_count > 0)
	{
		if (create_output_files(cmd) == -1 || create_append_files(cmd) == -1)
			return (-1);
		return (apply_final_redirection(cmd, process));
	}
	return (0);
}

static int	setup_input(t_parsed_cmd *cmd, t_process *process, t_exec *exec)
{
	int	fd;

	(void)exec;
	if (cmd->heredoc_count > 0)
		return (0);
	if (cmd->input_count > 0)
	{
		fd = handle_input_redir(cmd->input_file[cmd->input_count - 1]);
		if (fd == -1)
			return (-1);
		process->stdin_backup = dup(STDIN_FILENO);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return (0);
}

int	setup_redirections(t_parsed_cmd *cmd, t_process *process, t_exec *exec)
{
	int	input_error;

	if (!cmd)
		return (0);
	process->stdin_backup = -1;
	process->stdout_backup = -1;
	if (cmd->output_count > 0)
		create_output_files(cmd);
	if (cmd->append_count > 0)
		create_append_files(cmd);
	input_error = setup_input(cmd, process, exec);
	if (input_error == -1)
		return (-1);
	if (handle_output_redirs(cmd, process) == -1)
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
