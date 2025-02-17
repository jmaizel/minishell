/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/17 16:51:20 by jacobmaizel      ###   ########.fr       */
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

static void	execute_command(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	char		*cmd_path;
	char		*cmd_to_parse;

	if (!cmd->redirection)
		cmd->redirection = parse_redir(cmd->cmd_pipe);
	// Seulement les redirections non-heredoc ici
	if (cmd->redirection && setup_redirections(cmd->redirection,
			&exec->process) == -1)
		exit(1);
	if (cmd->redirection)
		cmd_to_parse = cmd->redirection->cmd;
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

static int	fork_and_execute(t_pip *cmd, t_exec *exec, int i, int pipes[][2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		setup_child_signals();
		setup_child_io(i, pipes, exec->pipe_count);
		execute_command(cmd, exec);
	}
	return (pid);
}

int exec_pipeline(t_pip *pipeline, t_exec *exec)
{
    int     pipes[1024][2];
    t_pip   *current;
    pid_t   *pids;
    int     i;
    int     ret;

    exec->pipe_count = count_pipes(pipeline);
    
    // Traiter tous les heredocs avant de commencer le pipeline
    current = pipeline;
    while (current)
    {
        if (!current->redirection)
            current->redirection = parse_redir(current->cmd_pipe);
        if (current->redirection && current->redirection->heredoc_count > 0)
        {
            ret = handle_heredoc(current->redirection->heredoc_delim, 
                               current->redirection->heredoc_count);
            if (ret == -1)
                return (1);
        }
        current = current->next;
    }

    if (setup_pipes(pipes, exec->pipe_count) == -1)
        return (1);
    pids = malloc(sizeof(pid_t) * (exec->pipe_count + 1));
    if (!pids)
    {
        close_all_pipes(pipes, exec->pipe_count);
        return (1);
    }
    current = pipeline;
    i = 0;
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
    setup_parent_signals();
    return (exec->exit_status);
}