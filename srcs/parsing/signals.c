/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:58:53 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/30 11:40:25 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// but de cette fonction :
// 1. Utilisation de readline qui gère automatiquement le prompt
// 2. // Si input est NULL (Ctrl-D), on retourne NULL
// 3. Ajouter la commande à l'historique si elle n'est pas vide

char	*get_user_input(void)
{
	char	*input;

	input = readline("~$ ");
	if (!input)
		return (NULL);
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		// Remplacer la ligne actuelle par une chaîne vide
		rl_replace_line("", 0);
		// Écrire un saut de ligne
		write(1, "\n", 1);
		// Préparer une nouvelle ligne
		rl_on_new_line();
		// Réafficher le prompt
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, handle_signal);
}