/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/27 16:05:06 by cdedessu         ###   ########.fr       */
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
    int     ret = 0;
    char    *expanded_cmd;
    t_pip   *current;
    int     heredoc_fd = -1;

    if (!cell || !cell->pipcell)
        return (1);
    init_exec_struct(&exec, tools);
    exec.pipe_count = count_pipes(cell->pipcell);

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

    t_parsed_cmd *redir = cell->pipcell->redirection;
    if (redir && redir->heredoc_count > 0)
    {
        heredoc_fd = handle_heredoc(redir, &exec);
        if (heredoc_fd == -1)
            return (1);
    }

    if (exec.pipe_count > 0)
    {
        ret = exec_pipeline(cell->pipcell, &exec, heredoc_fd);
        if (heredoc_fd != -1)
            close(heredoc_fd);
    }
    else
    {
        if (redir && redir->cmd && *redir->cmd)
        {
            if (heredoc_fd != -1)
            {
                exec.process.stdin_backup = dup(STDIN_FILENO);
                if (dup2(heredoc_fd, STDIN_FILENO) == -1)
                {
                    close(heredoc_fd);
                    return (1);
                }
                close(heredoc_fd);
            }
            expanded_cmd = expand_str(redir->cmd, tools);
            if (expanded_cmd)
            {
                free(redir->cmd);
                redir->cmd = expanded_cmd;
            }
            ret = exec_simple_cmd(cell->pipcell, &exec);
            if (heredoc_fd != -1)
                restore_redirections(&exec.process);
        }
        else if (heredoc_fd != -1)
        {
            close(heredoc_fd);
        }
    }
    if (exec.cmd_paths)
        free_str_array(exec.cmd_paths);
    return (ret);
}
