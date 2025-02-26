/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_display.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 17:37:56 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_redirect_files(char **files, int count, const char *label)
{
	int	i;

	if (count > 0)
	{
		printf(" %s:\n", label);
		i = 0;
		while (i < count)
		{
			printf(" %d: [%s]\n", i + 1, files[i]);
			i++;
		}
	}
}

static void	print_redirections(t_parsed_cmd *cmd, int type)
{
	if (type == 0)
		print_redirect_files(cmd->input_file, cmd->input_count, "Input File");
	else if (type == 1)
		print_redirect_files(cmd->output_file, cmd->output_count,
			"Output File");
	else if (type == 2)
		print_redirect_files(cmd->append_file, cmd->append_count,
			"Append File");
	else if (type == 3)
		print_redirect_files(cmd->heredoc_delim, cmd->heredoc_count,
			"Heredoc Delimiters");
}

void	print_parsed_command(t_parsed_cmd *cmd)
{
	printf(" parsing results:\n");
	printf(" Full command: [%s]\n", cmd->full_cmd);
	print_redirections(cmd, 0);
	print_redirections(cmd, 1);
	print_redirections(cmd, 2);
	print_redirections(cmd, 3);
}

static void	process_command(t_pip *current)
{
	t_parsed_cmd	*parsed_cmd;
	t_cmd_args		*cmd_args;

	parsed_cmd = parse_redir(current->cmd_pipe);
	if (parsed_cmd)
	{
		print_parsed_command(parsed_cmd);
		if (parsed_cmd->cmd && *(parsed_cmd->cmd) != '\0')
		{
			cmd_args = parse_command_args(parsed_cmd->cmd);
			if (cmd_args)
			{
				print_cmd_args(cmd_args);
				free_cmd_args(cmd_args);
			}
		}
		free_parsed_cmd(parsed_cmd);
	}
}

void	parsing_line(char *user_input, t_tools *tools)
{
	t_sep	*cell;
	t_pip	*current;

	if (!user_input || check_invalid_chars(user_input))
		return ;
	(void)tools;
	cell = create_cell(ft_strdup(user_input));
	if (!cell)
		return ;
	parse_pipes(cell);
	current = cell->pipcell;
	while (current && current->cmd_pipe)
	{
		process_command(current);
		current = current->next;
	}
	free_cell(cell);
}
