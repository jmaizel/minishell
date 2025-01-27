/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:17:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/27 13:57:10 by jmaizel          ###   ########.fr       */
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

// Fonction de débogage améliorée
void	print_list(t_sep *list)
{
	int				i;
	t_pip			*pipe_cell;
	int				j;
	t_redirection	*redir;

	i = 0;
	while (list)
	{
		printf("Commande %d: %s\n", i, list->cmd_sep);
		if (list->pipcell)
		{
			pipe_cell = list->pipcell;
			j = 0;
			while (pipe_cell)
			{
				printf("  Pipe %d: %s\n", j, pipe_cell->cmd_pipe);
				redir = pipe_cell->redirection;
				if (redir)
				{
					if (redir->input_file)
						printf("    Entrée: %s\n", redir->input_file);
					if (redir->output_file)
						printf("    Sortie: %s\n", redir->output_file);
					if (redir->append_file)
						printf("    Ajout: %s\n", redir->append_file);
				}
				pipe_cell = pipe_cell->next;
				j++;
			}
		}
		list = list->next;
		i++;
	}
}

// fonction princpiale pour le parsing
// 1. decouper les commandea avec ft_split
// 2. ajouter les cmmandes dans la liste chainee
// 3. stocker la liste dans tools pour usage ulterieur
// 4. afficher la liste pour deboguer
void	parsing_line(char *user_input, t_tools *tools)
{
	t_sep	*list;
	char	**cmds;
	int		i;

	list = NULL;
	cmds = ft_split(user_input, ';');
	if (!cmds)
	{
		printf("Erreur : ft_split a echoue./n");
		return ;
	}
	i = 0;
	while (cmds[i])
	{
		list = add_cell(list, cmds[i], i);
		i++;
	}
	tools->cmds = (struct s_simple_cmds *)list;
	print_list(list);
	free_str_array(cmds);
}
