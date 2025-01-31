/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 11:58:02 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_parsed_command(t_parsed_cmd *cmd)
{
	int	j;
	int	k;
	int	l;
	int	i;

	printf(" parsing results:\n");
	printf(" Full command: [%s]\n", cmd->full_cmd);
	if (cmd->input_count > 0)
	{
		printf(" Input File:\n");
		j = 0;
		while (j < cmd->input_count)
		{
			printf(" %d: [%s]\n", j + 1, cmd->input_file[j]);
			j++;
		}
	}
	if (cmd->output_count > 0)
	{
		printf(" Output File:\n");
		k = 0;
		while (k < cmd->output_count)
		{
			printf(" %d: [%s]\n", k + 1, cmd->output_file[k]);
			k++;
		}
	}
	if (cmd->append_count > 0)
	{
		printf(" Append File:\n");
		l = 0;
		while (l < cmd->append_count)
		{
			printf(" %d: [%s]\n", l + 1, cmd->append_file[l]);
			l++;
		}
	}
	if (cmd->heredoc_count > 0)
	{
		printf(" Heredoc Delimiters:\n");
		i = 0;
		while (i < cmd->heredoc_count)
		{
			printf(" %d: [%s]\n", i + 1, cmd->heredoc_delim[i]);
			i++;
		}
	}
}

void	free_parsed_cmd(t_parsed_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	free(cmd->full_cmd);
	free(cmd->cmd);
	i = 0;
	while (i < cmd->input_count)
	{
		free(cmd->input_file[i]);
		i++;
	}
	free(cmd->input_file);
	i = 0;
	while (i < cmd->output_count)
	{
		free(cmd->output_file[i]);
		i++;
	}
	free(cmd->output_file);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		free(cmd->heredoc_delim[i]);
		i++;
	}
	i = 0;
	while (i < cmd->append_count)
	{
		free(cmd->append_file[i]);
		i++;
	}
	free(cmd->heredoc_delim);
	free(cmd);
}

void	parsing_line(char *user_input, t_tools *tools)
{
	t_parsed_cmd *parsed_cmd;
	t_cmd_args *cmd_args;

	(void)tools;
	parsed_cmd = parse_redir(user_input);
	cmd_args = parse_command_args(parsed_cmd->cmd);
	print_parsed_command(parsed_cmd);
	print_cmd_args(cmd_args);
	free_parsed_cmd(parsed_cmd);
	free_cmd_args(cmd_args);
}