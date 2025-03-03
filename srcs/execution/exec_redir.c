/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 14:43:18 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	handle_output_redir(char *file, int append)
{
	int	fd;
	int	flags;

	if (!file)
	{
		ft_printf("minishell: redirection error: no file specified\n");
		return (-1);
	}
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		ft_printf("minishell: %s: Permission denied\n", file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	return (close(fd), 0);
}

static int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("minishell: %s: No such file or directory\n", file);
		return (-1);
	}
	return (fd);
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

static int	handle_intermediate_files(t_parsed_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->output_count - 1)
	{
		if (!cmd->output_file[i])
		{
			ft_printf("minishell: invalid output file at index %d\n", i);
			return (-1);
		}
		fd = open(cmd->output_file[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("minishell: %s: Permission denied\n",
				cmd->output_file[i]);
			return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	backup_and_redirect_stdout(void)
{
	int	stdout_backup;

	stdout_backup = dup(STDOUT_FILENO);
	if (stdout_backup == -1)
	{
		ft_printf("minishell: dup error\n");
		return (-1);
	}
	return (stdout_backup);
}

static int	handle_final_output_file(t_parsed_cmd *cmd)
{
	if (!cmd->output_file[cmd->output_count - 1])
	{
		ft_printf("minishell: invalid final output file\n");
		return (-1);
	}
	return (handle_output_redir(cmd->output_file[cmd->output_count - 1], 0));
}

static int	setup_output_redir(t_parsed_cmd *cmd, t_process *process)
{
	process->stdout_backup = backup_and_redirect_stdout();
	if (process->stdout_backup == -1)
	{
		ft_printf("minishell: dup error\n");
		return (-1);
	}
	if (handle_intermediate_files(cmd) == -1)
		return (-1);
	return (handle_final_output_file(cmd));
}

int	setup_redirections(t_parsed_cmd *cmd, t_process *process, t_exec *exec)
{
	if (!cmd)
		return (0);
	process->stdin_backup = -1;
	process->stdout_backup = -1;
	if (setup_input(cmd, process, exec) == -1)
		return (-1);
	if (cmd->output_count > 0)
	{
		if (setup_output_redir(cmd, process) == -1)
			return (-1);
	}
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
