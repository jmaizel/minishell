/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:58:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/16 14:00:25 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_prompt(t_tools *tools)
{
	char	*ligne;

	ligne = NULL;
	while (1)
	{
		ft_printf("~$ ");
		// Lire la ligne de commande
		if (get_next_line(0, &ligne) <= 0) // Si EOF ou erreur
		{
			free(ligne);
			break ; // Sortie de la boucle si EOF ou erreur
		}
		// Vérifier si la ligne est vide
		if (ligne[0] == '\0')
		{
			free(ligne);
			continue ; // Si la ligne est vide, on demande une nouvelle entrée
		}
		// Appel du parsing de la ligne
		parsing_ligne(ligne, tools);
		// Traitement de la commande après parsing
		traiter_commande(tools);
		// Libérer la mémoire de la ligne
		free(ligne);
		ligne = NULL;
		// Si la commande est "exit", on quitte la boucle
		if (ligne && ft_strcmp(ligne, "exit") == 0)
			break ;
	}
}
