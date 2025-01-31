/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 16:40:25 by cdedessu         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
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
	if (cmd->input_file)
		free_str_array(cmd->input_file);
	if (cmd->output_file)
		free_str_array(cmd->output_file);
	if (cmd->append_file)
		free_str_array(cmd->append_file);
	if (cmd->heredoc_delim)
		free_str_array(cmd->heredoc_delim);
	free(cmd);
}

void	cleanup_pip(t_pip *pip)
{
	t_pip	*next;

	while (pip)
	{
		next = pip->next;
		if (pip->cmd_pipe)
			free(pip->cmd_pipe);
		if (pip->redirection)
			cleanup_parsed_cmd(pip->redirection);
		free(pip);
		pip = next;
	}
}

void	cleanup_sep(t_sep *sep)
{
	t_sep	*next;

	while (sep)
	{
		next = sep->next;
		if (sep->cmd_sep)
			free(sep->cmd_sep);
		if (sep->pipcell)
			cleanup_pip(sep->pipcell);
		free(sep);
		sep = next;
	}
}
