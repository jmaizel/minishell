/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:58:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/27 12:00:11 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// but de cette fonction :
// 1. recuperer les chemins a partir de PATH au demarrage
// 2. lire les commandes utilisateur en boucle
// 3. passer les commandes au psring pour les executer
void	loop_prompt(t_tools *tools, char **env)
{
	char	*user_input;
	char	**paths;

	// Récupérer les chemins PATH
	paths = get_env_paths(env, "PATH");
	if (!paths)
	{
		ft_printf("Erreur : PATH non trouvé.\n");
		return ;
	}
	// Configuration de readline
	rl_catch_signals = 0; // Nous gérons nous-mêmes les signaux
	// Configurer les signaux
	setup_signals();
	while (1)
	{
		user_input = get_user_input();
		// Gestion de Ctrl-D (fin de fichier)
		if (!user_input)
		{
			ft_printf("\nExit\n");
			break ;
		}
		// Ignorer les lignes vides
		if (user_input[0] == '\0')
		{
			free(user_input);
			continue ;
		}
		// Traitement de la commande
		parsing_line(user_input, tools);
		// Libérer la mémoire
		free(user_input);
	}
	// Nettoyer l'historique à la fin
	rl_clear_history();
	free_str_array(paths);
}
