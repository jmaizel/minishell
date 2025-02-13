/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:15:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 21:09:45 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	close_all_pipes(int **pipes, int count)
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

static int	**init_pipes(int count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
	return (pipes);
}

static void	free_pipes(int **pipes, int count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < count)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

static int	setup_pipes(int **pipes, int count)
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

static void	handle_child_pipe(int i, int **pipes, int pipe_count, t_exec *exec)
{
	(void)exec;
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < pipe_count)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipes, pipe_count);
}

static void	execute_piped_command(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	char		*cmd_path;
	int			builtin_ret;

	setup_child_signals();
	builtin_ret = handle_builtin(cmd, exec);
	if (builtin_ret != -1)
		exit(builtin_ret);
	if (!cmd->redirection)
		cmd->redirection = parse_redir(cmd->cmd_pipe);
	if (cmd->redirection && setup_redirections(cmd->redirection, &exec->process)
		== -1)
		exit(1);
	args = parse_command_args(cmd->redirection ? cmd->redirection->cmd
			: cmd->cmd_pipe);
	if (!args || !args->argv[0])
	{
		if (args)
			free_cmd_args(args);
		exit(1);
	}
	cmd_path = get_cmd_path(args->argv[0], exec->cmd_paths);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_cmd_args(args);
		exit(127);
	}
	execve(cmd_path, args->argv, exec->tools->env);
	free(cmd_path);
	free_cmd_args(args);
	exit(127);
}

static int	validate_pipeline(int pipe_count)
{
	if (pipe_count > MAX_PIPES)
	{
		ft_putstr_fd("minishell: maximum pipe count exceeded\n", 2);
		return (0);
	}
	return (1);
}

static int	fork_and_execute(t_pip *cmd, t_exec *exec, int i, int **pipes)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		handle_child_pipe(i, pipes, exec->pipe_count, exec);
		execute_piped_command(cmd, exec);
	}
	return (pid);
}

static int	cleanup_pipeline(int **pipes, pid_t *pids, t_exec *exec)
{
	close_all_pipes(pipes, exec->pipe_count);
	free_pipes(pipes, exec->pipe_count);
	wait_all_processes(exec, exec->pipe_count + 1);
	free(pids);
	return (exec->exit_status);
}

int	exec_pipeline(t_pip *pipeline, t_exec *exec)
{
	int		**pipes;
	t_pip	*current;
	pid_t	*pids;
	int		i;

	if (!validate_pipeline(exec->pipe_count))
		return (1);
	pipes = init_pipes(exec->pipe_count);
	if (!pipes || setup_pipes(pipes, exec->pipe_count) == -1)
		return (1);
	pids = malloc(sizeof(pid_t) * (exec->pipe_count + 1));
	if (!pids)
	{
		free_pipes(pipes, exec->pipe_count);
		return (1);
	}
	current = pipeline;
	i = 0;
	while (current)
	{
		pids[i] = fork_and_execute(current, exec, i, pipes);
		if (pids[i] == -1)
		{
			free_pipes(pipes, exec->pipe_count);
			free(pids);
			return (1);
		}
		current = current->next;
		i++;
	}
	return (cleanup_pipeline(pipes, pids, exec));
}
