/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:06:14 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/03 14:36:22 by jmaizel          ###   ########.fr       */
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

static t_cmd_args	*get_command_args(t_pip *cmd)
{
	if (!cmd->redirection)
		cmd->redirection = parse_redir(cmd->cmd_pipe);
	if (cmd->redirection)
		return (parse_command_args(cmd->redirection->cmd));
	return (parse_command_args(cmd->cmd_pipe));
}

static char	*get_command_path(t_cmd_args *args, t_exec *exec)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(args->argv[0], exec->cmd_paths);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", args->argv[0]);
		free_cmd_args(args);
		exit(127);
	}
	return (cmd_path);
}

static void	execute_command(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	char		*cmd_path;

	args = get_command_args(cmd);
	if (!args || !args->argv[0])
	{
		if (args)
			free_cmd_args(args);
		exit(1);
	}
	cmd_path = get_command_path(args, exec);
	execve(cmd_path, args->argv, exec->tools->env);
	free(cmd_path);
	free_cmd_args(args);
	exit(127);
}

static void	duplicate_redirect_fd(t_exec_params *params)
{
	if (params->heredoc_fd != -1 && params->i == 0)
	{
		if (dup2(params->heredoc_fd, STDIN_FILENO) == -1)
			exit(1);
		close(params->heredoc_fd);
	}
	else if (params->i > 0)
	{
		if (dup2(params->pipes[params->i - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (params->i < params->exec->pipe_count)
	{
		if (dup2(params->pipes[params->i][1], STDOUT_FILENO) == -1)
			exit(1);
	}
}

static pid_t	fork_and_execute_child(t_exec_params *params)
{
	duplicate_redirect_fd(params);
	close_all_pipes(params->pipes, params->exec->pipe_count);
	if (params->cmd->redirection
		&& params->cmd->redirection->heredoc_count == 0)
	{
		if (setup_redirections(params->cmd->redirection, &params->exec->process,
				params->exec) == -1)
			exit(1);
	}
	execute_command(params->cmd, params->exec);
	return (0);
}

static pid_t	fork_and_execute(t_exec_params *params)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		if (params->heredoc_fd != -1)
			close(params->heredoc_fd);
		return (-1);
	}
	if (pid == 0)
	{
		fork_and_execute_child(params);
	}
	if (params->heredoc_fd != -1 && params->i == 0)
		close(params->heredoc_fd);
	return (pid);
}

static int	fork_and_execute_pid(t_fork_params *params)
{
	ft_memcpy(params->pipes, params->pipes, sizeof(int) * 1024 * 2);
	params->pids[params->i] = fork_and_execute((t_exec_params *)params);
	if (params->pids[params->i] == -1)
		return (1);
	return (0);
}

static int	cleanup_and_return(int pipes[][2], t_exec *exec, pid_t *pids)
{
	close_all_pipes(pipes, exec->pipe_count);
	wait_all_processes(exec, exec->pipe_count + 1);
	free(pids);
	return (exec->exit_status);
}

static int	exec_pipeline_pids(t_pip *pipeline, t_exec *exec, int pipes[][2],
		int heredoc_fd)
{
	t_pip			*current;
	pid_t			*pids;
	int				i;
	t_fork_params	params;

	current = pipeline;
	pids = malloc(sizeof(pid_t) * (exec->pipe_count + 1));
	if (!pids)
	{
		close_all_pipes(pipes, exec->pipe_count);
		return (1);
	}
	i = 0;
	while (current)
	{
		params.cmd = current;
		params.exec = exec;
		params.i = i;
		params.heredoc_fd = heredoc_fd;
		params.pids = pids;
		ft_memcpy(params.pipes, pipes, sizeof(int) * 1024 * 2);
		if (fork_and_execute_pid(&params))
		{
			free(pids);
			close_all_pipes(pipes, exec->pipe_count);
			return (1);
		}
		current = current->next;
		i++;
	}
	return (cleanup_and_return(pipes, exec, pids));
}

int	exec_pipeline(t_pip *pipeline, t_exec *exec, int heredoc_fd)
{
	int	pipes[1024][2];

	exec->pipe_count = count_pipes(pipeline);
	if (setup_pipes(pipes, exec->pipe_count) == -1)
		return (1);
	return (exec_pipeline_pids(pipeline, exec, pipes, heredoc_fd));
}
