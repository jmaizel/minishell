/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:54:14 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 17:05:56 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Récupère l'entrée utilisateur et affiche le prompt.
 * @return Une chaîne contenant la ligne de commande saisie.
 */
static char	*get_input(void)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
		ft_putendl_fd("exit", STDOUT_FILENO);
	else if (*line)
		add_history(line);
	return (line);
}

/**
 * @brief Exécute une ligne de commande en la passant d'abord par le parsing.
 * @param line La ligne de commande à analyser et exécuter.
 * @param tools Structure contenant l'environnement et les données du shell.
 */
static void	execute_command_line(char *line, t_tools *tools)
{
	t_pip *cmd;

	if (!line || !*line)
		return;

	// Parser la ligne pour obtenir `tools->cmds` (liste de `t_pip`)
	parsing_line(line, tools);

	// Vérifier si le parsing a réussi
	if (!tools->cmds)
	{
		ft_putendl_fd("Parsing error: invalid command", STDERR_FILENO);
		return;
	}

	cmd = (t_pip *)tools->cmds;

	// Exécuter la commande (simple ou pipeline)
	if (cmd->next)
		execute_pipeline(cmd, tools);
	else
		execute_simple_command(cmd, tools);

	// Nettoyer après exécution
	cleanup_pip(cmd);
	tools->cmds = NULL;
}

/**
 * @brief Copie l'environnement système pour l'utiliser dans le shell.
 * @param envp Tableau de chaînes représentant l'environnement.
 * @return Un nouveau tableau de chaînes alloué dynamiquement.
 */
char	**copy_env(char **envp)
{
	int		i;
	int		count;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;

	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);

	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

/**
 * @brief Libère la mémoire allouée pour l'environnement.
 * @param env Tableau de chaînes à libérer.
 */
static void	cleanup_env(char **env)
{
	int i = 0;

	if (!env)
		return;
	while (env[i])
		free(env[i++]);
	free(env);
}

/**
 * @brief Fonction principale du programme.
 */
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_tools	tools;

	(void)argc;
	(void)argv;
	ft_memset(&tools, 0, sizeof(t_tools));

	// Copier l'environnement
	tools.env = copy_env(envp);
	if (!tools.env)
	{
		ft_putendl_fd("Error: Failed to copy environment", STDERR_FILENO);
		return (1);
	}

	setup_signals();

	// Boucle principale du shell
	while (1)
	{
		line = get_input();
		if (!line)
			break;
		execute_command_line(line, &tools);
		free(line);
	}

	// Nettoyage avant de quitter
	cleanup_env(tools.env);
	return (tools.exit_code);
}



/* int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	t_tools tools;
	ft_memset(&tools, 0, sizeof(t_tools));
	tools.env = env;
	tools.exit_code = 0;
	tools.tokens = NULL;
	tools.cmds = NULL;
	loop_prompt(&tools, env);
	return (tools.exit_code);
} */