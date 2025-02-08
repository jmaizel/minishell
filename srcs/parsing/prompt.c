/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:58:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/08 11:27:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// but de cette fonction :
// 1. recuperer les chemins a partir de PATH au demarrage
// 2. on setup les signaux (comme crl-C qui interrpomt la commande en cours et affiche une nouvelle ligne de promopt)
// 3. gestion du ctrl-D(signale la fin de l entree utilisateur ,
//	c est pour quitter le programme)
// 4. on ignore les lignes vides
// 5. si la commande n est pas vide on l ajoute a lhistorique
// 7 traitement des autres commandes
// 8. on libere la memoire et on nettoie lhistoique
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
	setup_signals();
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
		free(user_input);
	}
	rl_clear_history();
	free_str_array(paths);
}
