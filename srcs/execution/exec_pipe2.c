/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*    exec_pipe_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:18:26 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 13:03:11 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	execute_command(t_pip *cmd, t_exec *exec)
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

void	duplicate_redirect_fd(t_exec_params *params)
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

int	handle_builtin_in_child(t_exec_params *params, t_cmd_args *args)
{
	int	ret;

	if (params->cmd->redirection && setup_redirections(params->cmd->redirection,
			&params->exec->process, params->exec) == -1)
	{
		free_cmd_args(args);
		exit(1);
	}
	ret = execute_builtin(args, params->exec);
	free_cmd_args(args);
	exit(ret);
	return (0);
}

pid_t	fork_and_execute_child(t_exec_params *params)
{
	t_cmd_args	*args;

	duplicate_redirect_fd(params);
	close_all_pipes(params->pipes, params->exec->pipe_count);
	args = get_command_args(params->cmd);
	if (args && args->argv[0] && is_builtin(args->argv[0]))
		handle_builtin_in_child(params, args);
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

pid_t	fork_and_execute2(t_exec_params *params)
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
