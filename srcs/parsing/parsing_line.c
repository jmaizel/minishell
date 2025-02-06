/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/06 16:29:41 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/execution.h"

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
void	execute_commands(t_sep *cell, t_tools *tools)
{
	t_env_manager	env_mgr;

	env_mgr.tools = tools;
	if (count_pipes(cell->pipcell) > 0)
		execute_pipeline(cell, tools, &env_mgr);
	else if (cell->pipcell && cell->pipcell->redirection)
		execute_simple_command(cell->pipcell, tools, &env_mgr);
}

void	parsing_line(char *user_input, t_tools *tools)
{
	t_sep			*cell;
	t_pip			*current;
	t_cmd_args		*cmd_args;
	t_parsed_cmd	*parsed_cmd;

	if (!user_input || check_invalid_chars(user_input))
	{
		ft_printf("Error: Invalid input\n");
		return ;
	}
	cell = create_cell(ft_strdup(user_input));
	if (!cell)
		return ;
	parse_pipes(cell);
	current = cell->pipcell;
	while (current && current->cmd_pipe)
	{
		parsed_cmd = parse_redir(current->cmd_pipe);
		if (parsed_cmd)
		{
			if (parsed_cmd->cmd)
			{
				cmd_args = parse_command_args(parsed_cmd->cmd);
				if (cmd_args)
					free_cmd_args(cmd_args);
			}
			free_parsed_cmd(parsed_cmd);
		}
		current = current->next;
	}
	execute_commands(cell, tools);
	free_cell(cell);
}