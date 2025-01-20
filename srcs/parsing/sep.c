/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:17:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/20 13:40:36 by jacobmaizel      ###   ########.fr       */
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

// Cette fonction parcourt la liste chaînée et affiche chaque cellule.
// Elle permet de vérifier que les commandes ont été correctement ajoutées à la liste.

void	print_list(t_sep *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("-----------------------------------\n");
		printf("| i = %d                            \n", i);
		printf("| list->cmd_sep : %s            \n", list->cmd_sep);
		printf("-----------------------------------\n");
		list = list->next;
		i++;
	}
}

void	parsing_line(char *user_input, t_tools *tools)
{
	t_sep	*list;
	char	**cmds;
	int		i;

	list = NULL;
	// etape 1 : decouper les commandes avec ft_split
	cmds = ft_split(user_input, ';');
	if (!cmds)
	{
		printf("Erreur : ft_split a echoue./n");
		return ;
	}
	// etape 2 : ajouter les commandes dans la liste chainee
	i = 0;
	while (cmds[i])
	{
		list = add_cell(list, cmds[i], i);
		i++;
	}
	// etape 3 : stocker la liste dans tools pour usage ult2rieur
	tools->cmds = (struct s_simple_cmds *)list;
	// etape 4 : afficher la liste pour deboguer
	print_list(list);
	free_str_array(cmds);
}

// fonction traiter_commande aura pour role d executer les commandes
// stockes dans la liste chainee apres leur parsing.

/* void	tratier_commande(t_tools *tools)
{
	t_sep	*cur;

	// etape 1 ; parcour la list cmds
	cur = (t_sep *)tools->cmds;
	while (cur)
	{
		// etape 2 : verifier si la commande contient des pipes
		if (ft_strchr(cur->cmd_sep, '|'))
		{
			parse_pipes(cur);
			execute_piped_commands(cur->pipcell);
		}
		else
			// etape 3 : executer la commande simple
			execute_command(cur->cmd_sep, tools);
		cur = cur->next;
	}
} */

// FONCTION A CREER :

// parse_pipe : decoupe cmd_sep avec ft_split '|' et rempli la sous_liste pipecell

// Execute_piped_commands : parcour la sous-liste pipcell et utilise pipe/fork/dup2
// pour connecter les commandes entre elles ,
//	donc en gros je vais refaire un pipex level up

// execute_command : but est de converir cmd_sep en untableau argv et executer la commande
// avec execve ,
//	cad le plan de la fonction va etre de verifier si la commande est un builtin,
// si ce n est pas le cas , chercher son chemin avec get_env_paths
