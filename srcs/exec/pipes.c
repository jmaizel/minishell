/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:41:59 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 12:58:25 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	close_unused_pipes(int prev_pipe, int pipe_fds[2])
{
	if (prev_pipe != -1)
		close(prev_pipe);
	close(pipe_fds[1]);
}

static void     handle_child_process(t_simple_cmds *cmd, int prev_pipe,
        int pipe_fds[2], t_tools *tools)
{
        if (prev_pipe != -1 && dup2(prev_pipe, STDIN_FILENO) == -1)
        {
                perror("dup2 failed");
                exit(EXIT_FAILURE);
        }
        if (cmd->next && dup2(pipe_fds[1], STDOUT_FILENO) == -1)
        {
                perror("dup2 failed");
                exit(EXIT_FAILURE);
        }
        close_unused_pipes(prev_pipe, pipe_fds);
        apply_redirections(cmd);
        setup_signals();  // Remplacez cette ligne
        execute_simple_command(cmd, tools);
        exit(tools->exit_code);
}

void execute_pipeline(t_simple_cmds *cmd, t_tools *tools)
{
    int pipe_fds[2];
    int prev_pipe;
    pid_t last_pid;
    int status;

    prev_pipe = -1;
    while (cmd)
    {
        if (cmd->next && pipe(pipe_fds) == -1)
            handle_error("pipe");

        pid_t pid = fork();
        if (pid == -1)
            handle_error("fork");
        else if (pid == 0)
        {
            setup_child_signals();  // Remplacez setup_signal_handlers()
            handle_child_process(cmd, prev_pipe, pipe_fds, tools);
        }
        else
        {
            cleanup_pipes(prev_pipe, pipe_fds, cmd->next != NULL);
            last_pid = pid;
        }
        cmd = cmd->next;
    }
    waitpid(last_pid, &status, 0);
    while (wait(NULL) > 0)
        ;
    if (WIFEXITED(status))
        tools->exit_code = WEXITSTATUS(status);
}
