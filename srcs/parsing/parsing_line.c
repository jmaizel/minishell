/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/30 13:31:38 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_parsed_command(t_parsed_cmd *cmd)
{
	printf(" parsing results:\n");
	printf(" Full command: [%s]\n", cmd->full_cmd);
	if (cmd->input_file)
		printf(" Input File: [%s]\n", cmd->input_file);
	if (cmd->output_file)
		printf(" Outpute file: [%s]\n", cmd->output_file);
	if (cmd->append_file)
		printf(" Append File: [%s]\n", cmd->append_file);
	if (cmd->heredoc_delim)
		printf(" Heredoc Delimiter: [%s]\n", cmd->heredoc_delim);
}

void	free_parsed_cmd(t_parsed_cmd *cmd)
{
	if (!cmd)
		return ;
	free(cmd->full_cmd);
	free(cmd->append_file);
	free(cmd->cmd);
	free(cmd->heredoc_delim);
	free(cmd->input_file);
	free(cmd->output_file);
}

void	parsing_line(char *user_input, t_tools *tools)
{
	t_parsed_cmd	*parsed_cmd;

	(void)tools;
	parsed_cmd = parse_command(user_input);
	print_parsed_command(parsed_cmd);
	free_parsed_cmd(parsed_cmd);
}