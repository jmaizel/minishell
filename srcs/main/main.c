/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:53 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/26 11:29:14 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/execution.h"

// Fonction pour dupliquer l’environnement
static char **dup_env(char **env)
{
    int count;
    char **new_env;
    int i;

    count = 0;
    while (env && env[count])
        count++;
    new_env = malloc(sizeof(char *) * (count + 1));
    if (!new_env)
        return (NULL);
    i = 0;
    while (i < count)
    {
        new_env[i] = ft_strdup(env[i]);
        if (!new_env[i])
        {
            while (i > 0)
                free(new_env[--i]);
            free(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[count] = NULL;
    return (new_env);
}

static void init_minishell(t_tools *tools, char **env)
{
    ft_memset(tools, 0, sizeof(t_tools));
    tools->env = dup_env(env); // Copie dynamique de l’environnement
    if (!tools->env)
    {
        ft_putstr_fd("minishell: failed to initialize environment\n", 2);
        exit(1);
    }
    tools->exit_code = 0;
    tools->tokens = NULL;
    tools->cmds = NULL;
}

void cleanup_minishell(t_tools *tools)
{
    int i;

    // Libère tools->env
    if (tools->env)
    {
        i = 0;
        while (tools->env[i])
        {
            free(tools->env[i]);
            i++;
        }
        free(tools->env);
    }
    rl_clear_history();
}

static int handle_execution(t_tools *tools, char *user_input)
{
    t_sep *cell;

    if (!user_input)
        return (1);
    // Vérification des syntaxes invalides (ex. pipe initial)
    if (user_input[0] == '|' || ft_strchr(user_input, '\n'))
    {
        ft_printf("minishell: syntax error near unexpected token '|'\n");
        return (1);
    }
    if (check_invalid_chars(user_input))
    {
        ft_printf("Error: Invalid input\n");
        return (1);
    }
    cell = create_cell(ft_strdup(user_input));
    if (!cell)
        return (1);
    parse_pipes(cell);
    if (cell && cell->pipcell)
    {
        setup_exec_signals();
        parsing_line(user_input, tools);
        exec_commands(cell, tools);
        restore_signals();
    }
    free_cell(cell);
    return (0);
}

int main(int argc, char **argv, char **env)
{
    t_tools tools;
    char *user_input;

    (void)argc;
    (void)argv;
    init_minishell(&tools, env);
    setup_interactive_signals();
    while (1)
    {
        g_signal_received = 0;
        user_input = get_user_input();
        if (!user_input || !ft_strncmp(user_input, "exit", 5))
        {
            if (user_input)
                free(user_input);
            break;
        }
        if (user_input[0] != '\0')
            handle_execution(&tools, user_input);
        free(user_input);
    }
    cleanup_minishell(&tools);
    return (tools.exit_code);
}
