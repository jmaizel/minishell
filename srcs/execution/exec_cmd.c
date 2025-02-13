/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:30 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 15:43:51 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"


static void    execute_cmd(t_pip *cmd, t_exec *exec, char *cmd_path)
{
    t_cmd_args  *args;
    char        *expanded_cmd;

    setup_child_signals();
    if (cmd->redirection)
    {
        if (setup_redirections(cmd->redirection, &exec->process) == -1)
            exit(1);
    }

    if (cmd->redirection)
        expanded_cmd = expand_str(cmd->redirection->cmd, exec->tools);
    else
        expanded_cmd = expand_str(cmd->cmd_pipe, exec->tools);

    if (!expanded_cmd)
        exit(1);

    args = parse_command_args(expanded_cmd);
    free(expanded_cmd);

    if (!args || !args->argv[0])
    {
        if (args)
            free_cmd_args(args);
        exit(1);
    }

    execve(cmd_path, args->argv, exec->tools->env);
    ft_printf("minishell: %s: command not found\n", args->cmd);
    free_cmd_args(args);
    exit(127);
}

int    exec_simple_cmd(t_pip *cmd, t_exec *exec)
{
    char        *cmd_path;
    int         status;
    t_cmd_args  *args;
    int         builtin_ret;
    char        *expanded_cmd;

    if (!cmd || (!cmd->cmd_pipe && !cmd->redirection))
        return (1);

    builtin_ret = handle_builtin(cmd, exec);
    if (builtin_ret != -1)
        return (builtin_ret);

    if (cmd->redirection)
        expanded_cmd = expand_str(cmd->redirection->cmd, exec->tools);
    else
        expanded_cmd = expand_str(cmd->cmd_pipe, exec->tools);

    if (!expanded_cmd)
        return (1);

    args = parse_command_args(expanded_cmd);
    free(expanded_cmd);

    if (!args || !args->argv[0])
    {
        if (args)
            free_cmd_args(args);
        return (1);
    }

    cmd_path = get_cmd_path(args->argv[0], exec->cmd_paths);
    free_cmd_args(args);

    if (!cmd_path)
        return (127);

    exec->process.pid = fork();
    if (exec->process.pid == -1)
        return (free(cmd_path), 1);

    if (exec->process.pid == 0)
        execute_cmd(cmd, exec, cmd_path);

    g_signal_received = 0;
    setup_parent_signals();
    waitpid(exec->process.pid, &status, 0);
    restore_signals();
    free(cmd_path);

    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
        {
            ft_putchar_fd('\n', STDERR_FILENO);
            return (130);
        }
        if (WTERMSIG(status) == SIGQUIT)
        {
            ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
            return (131);
        }
        return (128 + WTERMSIG(status));
    }
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}