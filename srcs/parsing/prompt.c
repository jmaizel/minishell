/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:58:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/20 13:38:07 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// but de cette fonction :
// 1. afficher le prompt ~$
// 2. lire  l'entree en utilisatateur avec get_next_line
// 3. retourner la commande entree par l utilisateur
char	*get_user_input(void)
{
	char	*input;

	write(1, "~$ ", 3);
	input = get_next_line(0);
	if (!input)
		return (NULL);
	return (input);
}

// but de cette fonction :
// 1. recuperer les chemins a partir de PATH au demarrage
// 2. lire les commandes utilisateur en boucle
// 3. passer les commandes au psring pour les executer
void	loop_prompt(t_tools *tools, char **env)
{
	char	*user_input;
	char	**paths;

	paths = get_env_paths(env, "PATH");
	if (!paths)
	{
		ft_printf("Erreur : PATH non trouvé.\n");
		return ;
	}
	while (1)
	{
		user_input = get_user_input();
		if (!user_input)
		{
			ft_printf("\nExit\n");
			break ;
		}
		if (user_input[0] == '\0')
		{
			free(user_input);
			continue ;
		}
		parsing_line(user_input, tools);
		//traiter_commande(tools, paths);
		free(user_input);
	}
	free_str_array(paths);
}
