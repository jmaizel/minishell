/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:17:29 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 16:29:47 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static int is_builtin(char *cmd)
{
    return (ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

static int execute_builtin(t_simple_cmds *cmd, t_tools *tools)
{
    int ret;

    if (ft_strcmp(cmd->str[0], "echo") == 0)
        ret = builtin_echo(cmd);
    else if (ft_strcmp(cmd->str[0], "cd") == 0)
        ret = builtin_cd(cmd, tools);
    else if (ft_strcmp(cmd->str[0], "pwd") == 0)
        ret = builtin_pwd();
    else if (ft_strcmp(cmd->str[0], "export") == 0)
        ret = builtin_export(cmd, tools);
    else if (ft_strcmp(cmd->str[0], "unset") == 0)
        ret = builtin_unset(cmd, tools);
    else if (ft_strcmp(cmd->str[0], "env") == 0)
        ret = builtin_env(tools);
    else if (ft_strcmp(cmd->str[0], "exit") == 0)
        ret = builtin_exit(cmd);
    else
        return (0);
    tools->exit_code = ret;
    return (1);
}

void execute_external_command(t_simple_cmds *cmd, t_tools *tools)
{
    char *path;
    pid_t pid;
    int status;

    // Trouver l'exécutable dans PATH
    path = find_executable(cmd->str[0], tools->env);
    if (!path)
    {
        ft_putstr_fd("Command not found: ", STDERR_FILENO);
        ft_putendl_fd(cmd->str[0], STDERR_FILENO);
        tools->exit_code = 127; // Code de retour pour commande introuvable
        return;
    }

    // Créer un processus enfant
    pid = fork();
    if (pid == 0) // Processus enfant
    {
        apply_redirections(cmd); // Appliquer les redirections si nécessaire
        if (execve(path, cmd->str, tools->env) == -1) // Exécuter la commande
        {
            perror("execve failed");
            exit(1); // Quitter l'enfant en cas d'erreur
        }
    }
    else if (pid > 0) // Processus parent
    {
        waitpid(pid, &status, 0); // Attendre la fin du processus enfant
        if (WIFEXITED(status)) // Récupérer le code de retour
            tools->exit_code = WEXITSTATUS(status);
    }
    else // Erreur dans fork
    {
        perror("fork failed");
        tools->exit_code = 1;
    }

    free(path); // Libérer la mémoire allouée pour l'exécutable
}


void execute_simple_command(t_simple_cmds *cmd, t_tools *tools)
{
    if (!cmd || !cmd->str || !cmd->str[0])
        return;
    if (is_builtin(cmd->str[0]))
    {
        execute_builtin(cmd, tools);
        return;
    }
    execute_external_command(cmd, tools);
}
