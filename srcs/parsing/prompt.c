/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:58:10 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/01/30 11:57:00 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// but de cette fonction :
// 1. recuperer les chemins a partir de PATH au demarrage
// 2. on setup les signaux
// 3. gestion du ctrl-D
// 4. on ignore les lignes vides
// 5. si la commande n est pas vide on l ajoute a lhistorique
// 6. on verifie la commande exit pour quitter le programme,
//	on regarde si il y a des arguments apres
// 7 traitement des autres commandes
// 8. on libere la memoire et on nettoie lhistoique
void	loop_prompt(t_tools *tools, char **env)
{
	char	*user_input;
	char	**paths;
	int		exit_shell;
	char	*trimmed;

	exit_shell = 0;
	// Récupérer les chemins PATH
	paths = get_env_paths(env, "PATH");
	if (!paths)
	{
		ft_printf("Erreur : PATH non trouvé.\n");
		return ;
	}
	setup_signals();
	while (!exit_shell)
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
		if (ft_strncmp(user_input, "exit", 4) == 0)
		{
			trimmed = ft_strtrim(user_input + 4, " \t");
			if (!trimmed || trimmed[0] == '\0')
			{
				exit_shell = 1;
				free(trimmed);
			}
			else
			{
				ft_printf("exit: too many arguments\n");
				free(trimmed);
			}
		}
		else
		{
			parsing_line(user_input, tools);
		}
		free(user_input);
	}
	rl_clear_history();
	free_str_array(paths);
}
