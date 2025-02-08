
#include "../../includes/minishell.h"
#include "../../includes/execution.h"

static void	init_minishell(t_tools *tools, char **env)
{
	ft_memset(tools, 0, sizeof(t_tools));
	tools->env = env;
	tools->exit_code = 0;
	tools->tokens = NULL;
	tools->cmds = NULL;
}

static void	cleanup_minishell(t_tools *tools)
{
	(void)tools;
	rl_clear_history();
}

static int	handle_execution(t_tools *tools, char *user_input)
{
	t_sep	*cell;

	// Vérification de l'entrée
	if (!user_input || check_invalid_chars(user_input))
	{
		ft_printf("Error: Invalid input\n");
		return (1);
	}

	// Créer la cellule pour le parsing
	cell = create_cell(ft_strdup(user_input));
	if (!cell)
		return (1);

	// Parser la ligne
	parse_pipes(cell);

	// Si le parsing a réussi, exécuter la commande
	if (cell && cell->pipcell)
	{
		// Laisser le parsing afficher ses informations
		parsing_line(user_input, tools);

		// En parallèle, exécuter la commande
		exec_commands(cell, tools);
	}

	// Nettoyer la mémoire
	free_cell(cell);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_tools	tools;
	char	*user_input;

	(void)argc;
	(void)argv;
	init_minishell(&tools, env);
	setup_parent_signals();

	while (1)
	{
		user_input = get_user_input();
		if (!user_input)
		{
			ft_printf("\nExit\n");
			break ;
		}
		if (user_input[0] != '\0')
			handle_execution(&tools, user_input);
		free(user_input);
	}

	cleanup_minishell(&tools);
	return (tools.exit_code);
}
