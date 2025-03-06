/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:25:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 16:00:52 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/execution.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd_args *args, t_exec *exec)
{
	int	result;

	if (!args || !args->argv[0])
		return (exec->tools->exit_code = 1);
	if (ft_strcmp(args->argv[0], "env") == 0)
		result = builtin_env(exec->tools, args->argv);
	else if (ft_strcmp(args->argv[0], "export") == 0)
		result = builtin_export(exec->tools, args->argv);
	else if (ft_strcmp(args->argv[0], "unset") == 0)
		result = builtin_unset(exec->tools, args->argv);
	else if (ft_strcmp(args->argv[0], "exit") == 0)
		result = builtin_exit(exec->tools, args->argv);
	else if (ft_strcmp(args->argv[0], "cd") == 0)
		result = builtin_cd(exec->tools, args->argv);
	else if (ft_strcmp(args->argv[0], "echo") == 0)
		result = builtin_echo(exec->tools, args->argv);
	else if (ft_strcmp(args->argv[0], "pwd") == 0)
		result = builtin_pwd(exec->tools, args->argv);
	else
		result = 1;
	exec->tools->exit_code = result;
	exec->exit_status = result;
	return (result);
}

t_cmd_args	*get_command_args(t_pip *cmd)
{
	t_cmd_args	*args;

	if (cmd->redirection)
		args = parse_command_args(cmd->redirection->cmd);
	else
		args = parse_command_args(cmd->cmd_pipe);
	return (args);
}

int	handle_builtin_redirection(t_pip *cmd, t_exec *exec)
{
	if (cmd->redirection)
	{
		if (setup_redirections(cmd->redirection, &exec->process, exec) == -1)
			return (-1);
	}
	return (0);
}

int	handle_builtin(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	int			ret;

	args = get_command_args(cmd);
	if (!args || !args->argv[0])
	{
		free_cmd_args(args);
		return (1);
	}
	if (!is_builtin(args->argv[0]))
	{
		free_cmd_args(args);
		return (-1);
	}
	if (handle_builtin_redirection(cmd, exec) == -1)
	{
		free_cmd_args(args);
		return (1);
	}
	ret = execute_builtin(args, exec);
	if (cmd->redirection)
		restore_redirections(&exec->process);
	free_cmd_args(args);
	return (ret);
}
