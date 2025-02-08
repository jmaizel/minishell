/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/08 17:43:41 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	handle_input_redir(t_parsed_cmd *cmd, t_process *process)
{
	int	fd;
	int	i;

	i = 0;
	while (i < cmd->input_count)
	{
		fd = open(cmd->input_file[i], O_RDONLY);
		if (fd == -1)
		{
			ft_printf("minishell: %s: No such file or directory\n",
				cmd->input_file[i]);
			return (-1);
		}
		if (i == cmd->input_count - 1)
		{
			process->stdin_backup = dup(STDIN_FILENO);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	handle_output_redir(t_parsed_cmd *cmd, t_process *process)
{
	int	fd;
	int	i;

	i = 0;
	while (i < cmd->output_count)
	{
		fd = open(cmd->output_file[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("minishell: %s: Permission denied\n",
				cmd->output_file[i]);
			return (-1);
		}
		if (i == cmd->output_count - 1)
		{
			process->stdout_backup = dup(STDOUT_FILENO);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	handle_append_redir(t_parsed_cmd *cmd, t_process *process)
{
	int	fd;
	int	i;

	i = 0;
	while (i < cmd->append_count)
	{
		fd = open(cmd->append_file[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_printf("minishell: %s: Permission denied\n",
				cmd->append_file[i]);
			return (-1);
		}
		if (i == cmd->append_count - 1)
		{
			process->stdout_backup = dup(STDOUT_FILENO);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	handle_heredocs(t_parsed_cmd *cmd, t_process *process)
{
	if (cmd->heredoc_count <= 0)
		return (0);
	process->stdin_backup = dup(STDIN_FILENO);
	if (handle_heredoc(cmd->heredoc_delim[cmd->heredoc_count - 1]) == -1)
		return (-1);
	return (0);
}

int	setup_redirections(t_parsed_cmd *cmd, t_process *process)
{
	if (!cmd)
		return (0);
	if (cmd->heredoc_count && handle_heredocs(cmd, process) == -1)
		return (-1);
	if (cmd->input_count && handle_input_redir(cmd, process) == -1)
		return (-1);
	if (cmd->output_count && handle_output_redir(cmd, process) == -1)
		return (-1);
	if (cmd->append_count && handle_append_redir(cmd, process) == -1)
		return (-1);
	return (0);
}

void	restore_redirections(t_process *process)
{
	if (process->stdin_backup != -1)
	{
		dup2(process->stdin_backup, STDIN_FILENO);
		close(process->stdin_backup);
		process->stdin_backup = -1;
	}
	if (process->stdout_backup != -1)
	{
		dup2(process->stdout_backup, STDOUT_FILENO);
		close(process->stdout_backup);
		process->stdout_backup = -1;
	}
}
