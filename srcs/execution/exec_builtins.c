/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:25:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/12 14:04:45 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/execution.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "env") == 0)
    return (1);
  if (ft_strcmp(cmd, "export") == 0)
    return (0);
	if (ft_strcmp(cmd, "unset") == 0)
    return (0);
  if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd_args *args, t_exec *exec)
{
	if (!args || !args->argv[0])
		return (1);
	if (ft_strcmp(args->argv[0], "env") == 0)
		return (builtin_env(exec->tools, args->argv));
	if (ft_strcmp(args->argv[0], "export") == 0)
		return (builtin_export(exec->tools, args->argv));
	if (ft_strcmp(args->argv[0], "unset") == 0)
		return (builtin_unset(exec->tools, args->argv));
	if (!ft_strcmp(args->argv[0], "exit"))
		return (builtin_exit(exec->tools, args->argv));
	if (ft_strcmp(args->argv[0], "cd") == 0)
		return (builtin_cd(exec->tools, args->argv));
	if (ft_strcmp(args->argv[0], "echo") == 0)
		return (builtin_echo(exec->tools, args->argv));
	if (ft_strcmp(args->argv[0], "pwd") == 0)
		return (builtin_pwd(exec->tools, args->argv));
	return (1);
}

int	handle_builtin(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	int			ret;

	args = parse_command_args(cmd->redirection
			? cmd->redirection->cmd : cmd->cmd_pipe);
	if (!args)
		return (1);
	if (!args->argv[0])
	{
		free_cmd_args(args);
		return (1);
	}
	if (!is_builtin(args->argv[0]))
	{
		free_cmd_args(args);
		return (-1);
	}
	if (cmd->redirection)
	{
		if (setup_redirections(cmd->redirection, &exec->process) == -1)
		{
			free_cmd_args(args);
			return (1);
		}
	}
	ret = execute_builtin(args, exec);
	if (cmd->redirection)
		restore_redirections(&exec->process);
	free_cmd_args(args);
	return (ret);
}