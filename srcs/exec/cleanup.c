/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/02 19:45:22 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	free_str_array_exec(char **array)
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
		free(cmd->full_cmd);
	if (cmd->cmd)
		free(cmd->cmd);
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
			free(current->cmd_pipe);
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
		free_str_array(args->argv);
	if (args->cmd)
		free(args->cmd);
	free(args);
}
