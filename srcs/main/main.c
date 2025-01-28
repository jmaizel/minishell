/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/28 10:45:54 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static char     *get_input(void)
{
        char    *line;

        line = readline("minishell> ");
        if (!line)
                ft_putendl_fd("exit", STDOUT_FILENO);
        else if (*line)
                add_history(line);
        return (line);
}

static void     execute_command_line(char *line, t_tools *tools)
{
        t_simple_cmds   cmd;
        char    **args;

        // Initialiser la structure avec memset
        ft_memset(&cmd, 0, sizeof(t_simple_cmds));
        
        // Allouer le tableau d'arguments
        args = malloc(2 * sizeof(char *));
        if (!args)
        {
                ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
                return;
        }
        
        // Copier la commande
        args[0] = ft_strdup(line);
        if (!args[0])
        {
                free(args);
                ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
                return;
        }
        args[1] = NULL;
        
        cmd.str = args;
        cmd.redirections = NULL;
        cmd.next = NULL;
        cmd.prev = NULL;

        execute_simple_command(&cmd, tools);

        // Nettoyer
        free(args[0]);
        free(args);
}

char    **copy_env(char **envp)
{
        int     i;
        int     count;
        char    **new_env;

        // Compter le nombre de variables d'environnement
        for (count = 0; envp[count]; count++)
                ;

        // Allouer le nouvel environnement
        new_env = malloc(sizeof(char *) * (count + 1));
        if (!new_env)
                return (NULL);

        // Copier chaque variable
        for (i = 0; i < count; i++)
        {
                new_env[i] = ft_strdup(envp[i]);
                if (!new_env[i])
                {
                        while (--i >= 0)
                                free(new_env[i]);
                        free(new_env);
                        return (NULL);
                }
        }
        new_env[count] = NULL;
        return (new_env);
}

int     main(int argc, char **argv, char **envp)
{
        char    *line;
        t_tools tools;

        (void)argc;
        (void)argv;

        // Initialiser la structure tools
        ft_memset(&tools, 0, sizeof(t_tools));
        
        // Copier l'environnement
        tools.env = copy_env(envp);
        if (!tools.env)
        {
                ft_putendl_fd("Error: Failed to copy environment", STDERR_FILENO);
                return (1);
        }

        // Configurer les signaux
        setup_signals();

        while (1)
        {
                line = get_input();
                if (!line)
                        break;
                execute_command_line(line, &tools);
                free(line);
        }

        // Nettoyer l'environnement
        if (tools.env)
        {
                char **tmp = tools.env;
                while (*tmp)
                        free(*tmp++);
                free(tools.env);
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
