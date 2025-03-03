/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:39 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:27:44 by jmaizel          ###   ########.fr       */
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
	close(fd);
	return (0);
}

int	setup_input(t_parsed_cmd *cmd, t_process *process, t_exec *exec)
{
	int	fd;

	(void)exec;
	if (cmd->heredoc_count > 0)
		return (0);
	if (cmd->input_count > 0)
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

int	open_intermediate_files(t_parsed_cmd *cmd)
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
