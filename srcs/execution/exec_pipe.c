/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:35:04 by jmaizel          ###   ########.fr       */
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

static int	setup_pipes(int pipes[][2], int count)
{
	int	i;

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

static void	execute_command(t_pip *cmd, t_exec *exec, int index)
{
	t_cmd_args	*args;
	char		*cmd_path;
	char		*cmd_to_parse;

	(void)index;
	setup_child_signals();
	if (!cmd->redirection)
		cmd->redirection = parse_redir(cmd->cmd_pipe);
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

static pid_t	fork_and_execute(t_pip *cmd, t_exec *exec, 
								int i, int pipes[][2], int heredoc_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		if (heredoc_fd != -1 && i == 0)
		{
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
				exit(1);
			close(heredoc_fd);
		}
		else if (i > 0)
		{
			if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
				exit(1);
		}
		if (i < exec->pipe_count)
		{
			if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
				exit(1);
		}
		close_all_pipes(pipes, exec->pipe_count);
		if (cmd->redirection && cmd->redirection->heredoc_count == 0)
		{
			if (setup_redirections(cmd->redirection, &exec->process, exec) == -1)
				exit(1);
		}
		execute_command(cmd, exec, i);
	}
	if (heredoc_fd != -1 && i == 0)
		close(heredoc_fd);
	return (pid);
}

int	exec_pipeline(t_pip *pipeline, t_exec *exec, int heredoc_fd)
{
	int		pipes[1024][2];
	t_pip	*current;
	pid_t	*pids;
	int		i;

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
		pids[i] = fork_and_execute(current, exec, i, pipes, heredoc_fd);
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