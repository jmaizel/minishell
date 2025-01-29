/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:17:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/29 14:52:40 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction qui creer une cellule de la liste chainnee
t_sep	*create_cell(char *cmd_sep)
{
	t_sep	*cell;

	cell = malloc(sizeof(t_sep));
	if (!cell)
		return (NULL);
	cell->prev = NULL;
	cell->next = NULL;
	cell->pipcell = NULL;
	cell->cmd_sep = cmd_sep;
	return (cell);
}

// Cette fonction ajoutera une cellule dans la liste chaînée à la position spécifiée.
// Si la liste est vide,
// la nouvelle cellule sera simplement le premier élément de la liste.

t_sep	*add_cell(t_sep *list, char *cmd_sep, int pos)
{
	t_sep	*prec;
	t_sep	*cur;
	t_sep	*cell;
	int		i;

	cell = create_cell(cmd_sep);
	if (!list)
		return (cell);
	cur = list;
	i = 0;
	while (i < pos && cur != NULL)
	{
		prec = cur;
		cur = cur->next;
		i++;
	}
	if (prec != NULL)
	{
		prec->next = cell;
		cell->prev = prec;
	}
	if (cur != NULL)
	{
		cell->next = cur;
		cur->prev = cell;
	}
	return (list);
}

void	print_command_details(t_sep *sep_cmd)
{
	t_pip			*pipe_cmd;
	int				pipe_index;
	t_redirection	*redir;

	printf("Command Separator: [%s]\n", sep_cmd->cmd_sep);
	pipe_cmd = sep_cmd->pipcell;
	pipe_index = 0;
	while (pipe_cmd)
	{
		printf("  Pipe Command %d: [%s]\n", pipe_index, pipe_cmd->cmd_pipe);
		redir = pipe_cmd->redirection;
		if (redir)
		{
			printf("    Redirections:\n");
			if (redir->input_file)
				printf("      Input File: %s\n", redir->input_file);
			if (redir->output_file)
				printf("      Output File: %s\n", redir->output_file);
			if (redir->append_file)
				printf("      Append File: %s\n", redir->append_file);
			if (redir->heredoc)
				printf("      Heredoc Delimiter: %s\n", redir->heredoc);
		}
		pipe_cmd = pipe_cmd->next;
		pipe_index++;
	}
}

// fonction princpiale pour le parsing
// 1. decouper les commandea avec ft_split
// 2. ajouter les cmmandes dans la liste chainee
// 3. stocker la liste dans tools pour usage ulterieur
// 4. afficher la liste pour deboguer
void	parsing_line(char *user_input, t_tools *tools)
{
	char	**semicolon_cmds;
	t_sep	*list;
	int		i;
	t_sep	*current_sep;

	printf("🔍 Parsing Input: [%s]\n", user_input);
	printf("==================================\n");
	// Séparer par ;
	semicolon_cmds = ft_split(user_input, ';');
	if (!semicolon_cmds)
	{
		printf("Erreur : ft_split a échoué.\n");
		return ;
	}
	list = NULL;
	i = 0;
	while (semicolon_cmds[i])
	{
		current_sep = create_cell(semicolon_cmds[i]);
		parse_pipes(current_sep);
		print_command_details(current_sep);
		list = add_cell(list, semicolon_cmds[i], i);
		i++;
	}
	tools->cmds = (struct s_simple_cmds *)list;
	free_str_array(semicolon_cmds);
	printf("==================================\n");
}
