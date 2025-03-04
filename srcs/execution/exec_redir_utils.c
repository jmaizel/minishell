/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:49:32 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 20:52:21 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int	handle_output_redir(char *file, int append)
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

int	handle_input_redir(char *file)
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

int	create_output_files(t_parsed_cmd *cmd)
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

int	create_append_files(t_parsed_cmd *cmd)
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

int	backup_stdout(t_process *process)
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
