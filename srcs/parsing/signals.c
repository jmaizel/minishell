/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:58:53 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/29 12:05:29 by jacobmaizel      ###   ########.fr       */
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
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, handle_signal);
}