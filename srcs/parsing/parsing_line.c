/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 13:37:43 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_redirections(t_parsed_cmd *cmd, int type)
{
	int	i;

	i = 0;
	if (type == 0 && cmd->input_count > 0)
	{
		printf(" Input File:\n");
		while (i < cmd->input_count)
		{
			printf(" %d: [%s]\n", i + 1, cmd->input_file[i]);
			i++;
		}
	}
	else if (type == 1 && cmd->output_count > 0)
	{
		printf(" Output File:\n");
		while (i < cmd->output_count)
		{
			printf(" %d: [%s]\n", i + 1, cmd->output_file[i]);
			i++;
		}
	}
	else if (type == 2 && cmd->append_count > 0)
	{
		printf(" Append File:\n");
		while (i < cmd->append_count)
		{
			printf(" %d: [%s]\n", i + 1, cmd->append_file[i]);
			i++;
		}
	}
	else if (type == 3 && cmd->heredoc_count > 0)
	{
		printf(" Heredoc Delimiters:\n");
		while (i < cmd->heredoc_count)
		{
			printf(" %d: [%s]\n", i + 1, cmd->heredoc_delim[i]);
			i++;
		}
	}
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

static void	free_redir_arrays(t_parsed_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->input_count)
		free(cmd->input_file[i++]);
	free(cmd->input_file);
	i = 0;
	while (i < cmd->output_count)
		free(cmd->output_file[i++]);
	free(cmd->output_file);
	i = 0;
	while (i < cmd->append_count)
		free(cmd->append_file[i++]);
	free(cmd->append_file);
	i = 0;
	while (i < cmd->heredoc_count)
		free(cmd->heredoc_delim[i++]);
	free(cmd->heredoc_delim);
}

void	free_parsed_cmd(t_parsed_cmd *cmd)
{
	if (!cmd)
		return ;
	free(cmd->full_cmd);
	free(cmd->cmd);
	free_redir_arrays(cmd);
	free(cmd);
}

void	free_cell(t_sep *cell)
{
	t_pip	*current;
	t_pip	*next;

	if (!cell)
		return ;
	current = cell->pipcell;
	while (current)
	{
		next = current->next;
		if (current->cmd_pipe)
			free(current->cmd_pipe);
		free(current);
		current = next;
	}
	if (cell->cmd_sep)
		free(cell->cmd_sep);
	free(cell);
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