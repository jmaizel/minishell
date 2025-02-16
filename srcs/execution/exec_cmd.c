/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:15:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/15 09:45:29 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_cmd_execution(t_pip *cmd, t_exec *exec, char *cmd_path)
{
	t_cmd_args	*args;

	setup_child_signals();
	if (cmd->redirection && setup_redirections(cmd->redirection, &exec->process)
		== -1)
		exit(1);
	// Vérifier si on a une redirection valide
	args = parse_command_args(cmd->redirection ? 
		(cmd->redirection->cmd ? cmd->redirection->cmd : cmd->cmd_pipe)
		: cmd->cmd_pipe);
	if (!args || !args->argv[0])
	{
		if (args)
			free_cmd_args(args);
		exit(1);
	}
	execve(cmd_path, args->argv, exec->tools->env);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(args->cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free_cmd_args(args);
	exit(127);
}

static int	handle_execution_status(int status)
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

static char	*prepare_command(t_pip *cmd, t_exec *exec, t_cmd_args **args)
{
	char	*cmd_path;
	char	*cmd_to_parse;
	
	cmd_to_parse = cmd->redirection ? 
		(cmd->redirection->cmd ? cmd->redirection->cmd : cmd->cmd_pipe)
		: cmd->cmd_pipe;

	*args = parse_command_args(cmd_to_parse);
	if (!*args || !(*args)->argv[0])
	{
		if (*args)
			free_cmd_args(*args);
		return (NULL);
	}
	cmd_path = get_cmd_path((*args)->argv[0], exec->cmd_paths);
	free_cmd_args(*args);
	return (cmd_path);
}

int	exec_simple_cmd(t_pip *cmd, t_exec *exec)
{
	char		*cmd_path;
	int			status;
	t_cmd_args	*args;
	int			builtin_ret;

	if (!cmd || (!cmd->cmd_pipe && !cmd->redirection))
		return (1);
	builtin_ret = handle_builtin(cmd, exec);
	if (builtin_ret != -1)
		return (builtin_ret);
	if (!(cmd_path = prepare_command(cmd, exec, &args)))
		return (127);
	exec->process.pid = fork();
	if (exec->process.pid == -1)
		return (free(cmd_path), 1);
	if (exec->process.pid == 0)
		handle_cmd_execution(cmd, exec, cmd_path);
	g_signal_received = 0;
	setup_parent_signals();
	waitpid(exec->process.pid, &status, 0);
	restore_signals();
	free(cmd_path);
	return (handle_execution_status(status));
}
