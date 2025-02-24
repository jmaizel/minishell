/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/24 21:01:16 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void    init_exec_struct(t_exec *exec, t_tools *tools)
{
    exec->tools = tools;
    exec->exit_status = 0;
    exec->pipe_count = 0;
    exec->process.stdin_backup = -1;
    exec->process.stdout_backup = -1;
    exec->process.pid = -1;
    exec->cmd_paths = get_env_paths(tools->env, "PATH");
}

int exec_commands(t_sep *cell, t_tools *tools)
{
    t_exec  exec;
    int     ret;
    char    *expanded_cmd;
    t_pip   *current;

    if (!cell || !cell->pipcell)
        return (1);
    init_exec_struct(&exec, tools);
    exec.pipe_count = count_pipes(cell->pipcell);

    // Parse and expand all pipeline commands
    current = cell->pipcell;
    while (current)
    {
        expanded_cmd = expand_str(current->cmd_pipe, tools);
        if (expanded_cmd)
        {
            free(current->cmd_pipe);
            current->cmd_pipe = expanded_cmd;
        }
        if (!current->redirection)
            current->redirection = parse_redir(current->cmd_pipe);
        current = current->next;
    }

    if (exec.pipe_count > 0)
        ret = exec_pipeline(cell->pipcell, &exec);
    else
    {
        if (cell->pipcell->redirection && cell->pipcell->redirection->cmd)
        {
            expanded_cmd = expand_str(cell->pipcell->redirection->cmd, tools);
            if (expanded_cmd)
            {
                free(cell->pipcell->redirection->cmd);
                cell->pipcell->redirection->cmd = expanded_cmd;
            }
        }
        ret = exec_simple_cmd(cell->pipcell, &exec);
    }
    if (exec.cmd_paths)
        free_str_array(exec.cmd_paths);
    return (ret);
}
