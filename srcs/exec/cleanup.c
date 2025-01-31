/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 15:21:27 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

void	cleanup_parsed_cmd(t_parsed_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->full_cmd)
	{
		free(cmd->full_cmd);
		cmd->full_cmd = NULL;
	}
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	free_str_array(cmd->input_file);
	free_str_array(cmd->output_file);
	free_str_array(cmd->append_file);
	free_str_array(cmd->heredoc_delim);
	free(cmd);
}

void	cleanup_pip(t_pip *pip)
{
	t_pip	*current;
	t_pip	*next;

	current = pip;
	while (current)
	{
		next = current->next;
		if (current->cmd_pipe)
		{
			free(current->cmd_pipe);
			current->cmd_pipe = NULL;
		}
		if (current->redirection)
			cleanup_parsed_cmd(current->redirection);
		free(current);
		current = next;
	}
}

void	cleanup_cmd_args(t_cmd_args *args)
{
	if (!args)
		return ;
	if (args->argv)
	{
		free_str_array(args->argv);
		args->argv = NULL;
	}
	if (args->cmd)
	{
		free(args->cmd);
		args->cmd = NULL;
	}
	free(args);
}