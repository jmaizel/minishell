/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/08 17:43:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	close_unused_pipes(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

static void	execute_first_cmd(t_pip *cmd, t_exec *exec, int pipe_fd[2])
{
	char		*cmd_path;
	t_cmd_args	*args;

	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);

	if (cmd->redirection)
		setup_redirections(cmd->redirection, &exec->process);

	args = parse_command_args(cmd->redirection ? cmd->redirection->cmd
			: cmd->cmd_pipe);
	if (!args || !args->argv[0])
		exit(1);

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

static void	execute_last_cmd(t_pip *cmd, t_exec *exec, int pipe_fd[2])
{
	char		*cmd_path;
	t_cmd_args	*args;

	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	if (cmd->redirection)
		setup_redirections(cmd->redirection, &exec->process);

	args = parse_command_args(cmd->redirection ? cmd->redirection->cmd
			: cmd->cmd_pipe);
	if (!args || !args->argv[0])
		exit(1);

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

int	exec_pipeline(t_pip *pipeline, t_exec *exec)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (1);

	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		execute_first_cmd(pipeline, exec, pipe_fd);

	pid2 = fork();
	if (pid2 < 0)
	{
		close_unused_pipes(pipe_fd);
		return (1);
	}
	if (pid2 == 0)
		execute_last_cmd(pipeline->next, exec, pipe_fd);

	close_unused_pipes(pipe_fd);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
