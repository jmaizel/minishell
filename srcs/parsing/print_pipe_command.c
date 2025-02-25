/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pipe_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:52:58 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/25 14:12:09 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_input_files(t_parsed_cmd *cmd)
{
	int	i;

	if (cmd->input_count <= 0)
		return ;
	ft_printf("Input files:\n");
	i = 0;
	while (i < cmd->input_count)
	{
		ft_printf(" - [%s]\n", cmd->input_file[i]);
		i++;
	}
}

static void	print_output_files(t_parsed_cmd *cmd)
{
	int	i;

	if (cmd->output_count <= 0)
		return ;
	ft_printf("Output files:\n");
	i = 0;
	while (i < cmd->output_count)
	{
		ft_printf(" - [%s]\n", cmd->output_file[i]);
		i++;
	}
}

static void	print_heredoc_delims(t_parsed_cmd *cmd)
{
	int	i;

	if (cmd->heredoc_count <= 0)
		return ;
	ft_printf("Heredoc delimiters:\n");
	i = 0;
	while (i < cmd->heredoc_count)
	{
		ft_printf(" - [%s]\n", cmd->heredoc_delim[i]);
		i++;
	}
}

static void	print_append_files(t_parsed_cmd *cmd)
{
	int	i;

	if (cmd->append_count <= 0)
		return ;
	ft_printf("Append files:\n");
	i = 0;
	while (i < cmd->append_count)
	{
		ft_printf(" - [%s]\n", cmd->append_file[i]);
		i++;
	}
}

void	print_pipe_command(t_pip *pipe_cmd, int pipe_num)
{
	if (!pipe_cmd || !pipe_cmd->redirection)
		return ;
	ft_printf("\n=== Pipe command %d ===\n", pipe_num);
	ft_printf("Command: [%s]\n", pipe_cmd->cmd_pipe);
	print_input_files(pipe_cmd->redirection);
	print_output_files(pipe_cmd->redirection);
	print_heredoc_delims(pipe_cmd->redirection);
	print_append_files(pipe_cmd->redirection);
}
