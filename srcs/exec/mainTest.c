/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainTest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:52:36 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 11:32:48 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	main(int argc, char **argv, char **envp)
{
	t_simple_cmds	cmd;
	t_lexer			redir1, redir2;
	t_tools			tools;

	(void)argc;
	(void)argv;

	// Initialisation des redirections
	redir1.token.type = TOK_INPUT_REDIR;
	redir1.token.value = "input.txt";
	redir1.next = &redir2;
	redir1.prev = NULL;

	redir2.token.type = TOK_OUTPUT_REDIR;
	redir2.token.value = "output.txt";
	redir2.next = NULL;
	redir2.prev = &redir1;

	// Initialisation de la commande
	cmd.str = (char *[]){"cat", NULL};
	cmd.redirections = &redir1;
	cmd.next = NULL;
	cmd.prev = NULL;

	// Initialisation de l'environnement
	tools.env = envp;

	// Exécution
	apply_redirections(&cmd);
	execute_simple_command(&cmd, &tools);
	return (0);
}