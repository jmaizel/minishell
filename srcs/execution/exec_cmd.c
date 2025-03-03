/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:30 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:34:56 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	execute_cmd(t_pip *cmd, t_exec *exec, char *cmd_path)
{
	t_cmd_args	*args;

	setup_child_signals();
	if (cmd->redirection)
	{
		if (setup_redirections(cmd->redirection, &exec->process, exec) == -1)
			exit(1);
	}
	args = parse_command_args(cmd->redirection ? 
			cmd->redirection->cmd : cmd->cmd_pipe);
	if (!args || !args->argv[0])
	{
		if (args)
			free_cmd_args(args);
		exit(1);
	}
	execve(cmd_path, args->argv, exec->tools->env);
	ft_printf("minishell: %s: command not found\n", args->argv[0]);
	free_cmd_args(args);
	exit(127);
}

static int	handle_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			return (130);
		}
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			return (131);
		}
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	fork_and_execute(t_pip *cmd, t_exec *exec, char *cmd_path)
{
	int	status;

	exec->process.pid = fork();
	if (exec->process.pid == -1)
	{
		free(cmd_path);
		return (1);
	}
	if (exec->process.pid == 0)
		execute_cmd(cmd, exec, cmd_path);
	g_signal_received = 0;
	setup_parent_signals();
	waitpid(exec->process.pid, &status, 0);
	restore_signals();
	free(cmd_path);
	return (handle_status(status));
}

static int	prepare_external_command(t_pip *cmd, t_exec *exec)
{
	char		*expanded_cmd;
	t_cmd_args	*args;
	char		*cmd_path;

	if (cmd->redirection)
		expanded_cmd = expand_str(cmd->redirection->cmd, exec->tools);
	else
		expanded_cmd = expand_str(cmd->cmd_pipe, exec->tools);
	if (!expanded_cmd)
		return (1);
	args = parse_command_args(expanded_cmd);
	free(expanded_cmd);
	if (!args || !args->argv[0])
	{
		if (args)
			free_cmd_args(args);
		return (1);
	}
	cmd_path = get_cmd_path(args->argv[0], exec->cmd_paths);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", args->argv[0]);
		free_cmd_args(args);
		return (127);
	}
	free_cmd_args(args);
	return (fork_and_execute(cmd, exec, cmd_path));
}

int	exec_simple_cmd(t_pip *cmd, t_exec *exec)
{
	int	builtin_ret;

	if (!cmd || (!cmd->cmd_pipe && !cmd->redirection))
		return (1);
	builtin_ret = handle_builtin(cmd, exec);
	if (builtin_ret != -1)
		return (builtin_ret);
	return (prepare_external_command(cmd, exec));
}