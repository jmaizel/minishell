/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/17 14:06:38 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "../includes/execution.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_tools		tools;
	t_simple_cmds	cmd;

	(void)argc;
	(void)argv;

	// Initialisation des outils
	tools.env = envp;
	tools.exit_code = 0;

	// Boucle principale du shell
	while (1)
	{
		// Afficher le prompt et lire la commande utilisateur
		line = readline("minishell> ");
		if (!line) // Gestion de Ctrl-D
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		add_history(line); // Ajouter la commande à l'historique
		
		// Parsing de la commande
		// Ici, vous pouvez appeler une fonction `parse_input` pour analyser `line`
		// Exemple d'initialisation manuelle (temporaire) :
		cmd.str = (char *[]){line, NULL};
		cmd.redirections = NULL; // À configurer si nécessaire
		cmd.next = NULL;
		cmd.prev = NULL;

		// Exécution de la commande
		apply_redirections(&cmd); // Gérer les redirections (temporairement vide si pas de redirections)
		execute_simple_command(&cmd, &tools);

		// Nettoyage
		free(line);
	}
	return (tools.exit_code);
}

/* int main(void)
{
	char *test1 = "echo \"Hello 'world'\""; // Toutes les quotes sont fermées
	char *test2 = "echo 'Hello world";      // Single quote non fermée
	char *test3 = "echo \"Hello world";     // Double quote non fermée

	printf("Test 1: %d\n", is_quote_closed(test1)); // 1
	printf("Test 2: %d\n", is_quote_closed(test2)); // 0
	printf("Test 3: %d\n", is_quote_closed(test3)); // 0

	return (0);
} */
