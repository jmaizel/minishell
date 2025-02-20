/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/20 20:55:06 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	close_all_pipes(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipes[i][0] != -1)
			close(pipes[i][0]);
		if (pipes[i][1] != -1)
			close(pipes[i][1]);
		i++;
	}
}

static void	init_pipes(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
}

static int	setup_pipes(int pipes[][2], int count)
{
	int	i;

	init_pipes(pipes, count);
	i = 0;
	while (i < count)
	{
		if (pipe(pipes[i]) == -1)
		{
			close_all_pipes(pipes, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	setup_child_io(int i, int pipes[][2], int pipe_count)
{
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (i < pipe_count)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			exit(1);
	}
	close_all_pipes(pipes, pipe_count);
}

static void execute_command(t_pip *cmd, t_exec *exec, int index)
{
    t_cmd_args  *args;
    char        *cmd_path;
    char        *cmd_to_parse;

	(void)index;
    setup_child_signals();
    if (!cmd->redirection)
        cmd->redirection = parse_redir(cmd->cmd_pipe);
    if (cmd->redirection)
    {
        if (setup_redirections(cmd->redirection, &exec->process) == -1)
            exit(1);
        cmd_to_parse = cmd->redirection->cmd;
    }
    else
        cmd_to_parse = cmd->cmd_pipe;
    args = parse_command_args(cmd_to_parse);
    if (!args || !args->argv[0])
    {
        if (args)
            free_cmd_args(args);
        exit(1);
    }
    cmd_path = get_cmd_path(args->argv[0], exec->cmd_paths);
    if (!cmd_path)
    {
        ft_printf("minishell: %s: command not found\n", args->argv[0]);
        free_cmd_args(args);
        exit(127);
    }
    execve(cmd_path, args->argv, exec->tools->env);
    free(cmd_path);
    free_cmd_args(args);
    exit(127);
}

static int handle_heredoc_pipe(t_pip *cmd, t_exec *exec, int i, int pipes[][2])
{
    pid_t pid;
    int fd;

    // Gérer d'abord le heredoc
    if (cmd->redirection && cmd->redirection->heredoc_count > 0)
    {
        if (cmd->redirection->heredoc_count > 1)
            fd = handle_heredoc_multiple(cmd->redirection);
        else
            fd = handle_heredoc(cmd->redirection->heredoc_delim[0]);
        if (fd == -1)
            return (-1);
    }

    pid = fork();
    if (pid == -1)
        return (-1);

    if (pid == 0)
    {
        setup_child_signals();
        
        // Fermer tous les autres pipes sauf celui en cours
        for (int j = 0; j < exec->pipe_count; j++)
        {
            if (j != i)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
        }

        // Configurer les redirection stdin/stdout
        if (i > 0)
            dup2(pipes[i - 1][0], STDIN_FILENO);
        if (i < exec->pipe_count)
            dup2(pipes[i][1], STDOUT_FILENO);

        if (cmd->redirection)
        {
            if (setup_redirections(cmd->redirection, &exec->process) == -1)
                exit(1);
        }

        execute_command(cmd, exec, i);
        exit(1);
    }

    if (fd > 0)
        close(fd);

    return pid;
}

static int fork_and_execute(t_pip *cmd, t_exec *exec, int i, int pipes[][2])
{
    pid_t pid;

    // Cas spécial pour heredoc
    if (cmd->redirection && cmd->redirection->heredoc_count > 0)
        return handle_heredoc_pipe(cmd, exec, i, pipes);

    pid = fork();
    if (pid == -1)
        return (-1);

    if (pid == 0)
    {
        setup_child_signals();
        setup_child_io(i, pipes, exec->pipe_count);
        
        if (cmd->redirection)
        {
            if (setup_redirections(cmd->redirection, &exec->process) == -1)
                exit(1);
        }
        
        execute_command(cmd, exec, i);
    }

    return (pid);
}

int exec_pipeline(t_pip *pipeline, t_exec *exec)
{
    int     pipes[1024][2];
    t_pip   *current;
    pid_t   *pids;
    int     i;

    exec->pipe_count = count_pipes(pipeline);
    if (setup_pipes(pipes, exec->pipe_count) == -1)
        return (1);

    pids = malloc(sizeof(pid_t) * (exec->pipe_count + 1));
    if (!pids)
    {
        close_all_pipes(pipes, exec->pipe_count);
        return (1);
    }

    i = 0;
    current = pipeline;
    while (current)
    {
        pids[i] = fork_and_execute(current, exec, i, pipes);
        if (pids[i] == -1)
        {
            free(pids);
            close_all_pipes(pipes, exec->pipe_count);
            return (1);
        }
        current = current->next;
        i++;
    }

    close_all_pipes(pipes, exec->pipe_count);
    wait_all_processes(exec, exec->pipe_count + 1);
    free(pids);
    return (exec->exit_status);
}
