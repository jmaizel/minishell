/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/22 21:13:17 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	handle_output_redir(char *file, int append)
{
	int		fd;
	int		flags;

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
	close(fd);
	return (0);
}

static int setup_input(t_parsed_cmd *cmd, t_process *process, t_exec *exec)
{
    int fd;

    if (cmd->heredoc_count > 0)
    {
        if (cmd->heredoc_count > 1)
            fd = handle_heredoc_multiple(cmd, exec);
        else
            fd = handle_heredoc(cmd->heredoc_delim[0]);
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
    else if (cmd->input_count > 0)
    {
        fd = open(cmd->input_file[cmd->input_count - 1], O_RDONLY);
        if (fd == -1)
        {
            ft_printf("minishell: %s: No such file or directory\n",
                cmd->input_file[cmd->input_count - 1]);
            return (-1);
        }
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
	if (!cmd)
		return (0);
	process->stdin_backup = -1;
	process->stdout_backup = -1;
	if (setup_input(cmd, process, exec) == -1)
		return (-1);
	if (cmd->append_count > 0 || cmd->output_count > 0)
	{
		process->stdout_backup = dup(STDOUT_FILENO);
		if (cmd->append_count > 0)
		{
			if (handle_output_redir(cmd->append_file[cmd->append_count - 1], 1)
				== -1)
				return (-1);
		}
		else
		{
			if (handle_output_redir(cmd->output_file[cmd->output_count - 1], 0)
				== -1)
				return (-1);
		}
	}
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
