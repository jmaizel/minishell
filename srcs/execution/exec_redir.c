/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 13:13:26 by jmaizel          ###   ########.fr       */
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

static int	create_output_files(t_parsed_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->output_count)
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

static int	create_append_files(t_parsed_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->append_count)
	{
		if (!cmd->append_file[i])
		{
			ft_printf("minishell: invalid append file at index %d\n", i);
			return (-1);
		}
		fd = open(cmd->append_file[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_printf("minishell: %s: Permission denied\n",
				cmd->append_file[i]);
			return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	backup_stdout(t_process *process)
{
	if (process->stdout_backup == -1)
	{
		process->stdout_backup = dup(STDOUT_FILENO);
		if (process->stdout_backup == -1)
		{
			ft_printf("minishell: dup error\n");
			return (-1);
		}
	}
	return (0);
}

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
