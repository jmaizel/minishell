/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:25:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 20:58:05 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/execution.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	return (0);
}

static int	execute_builtin_cmd(char *cmd, t_cmd_args *args, t_exec *exec)
{
	if (!ft_strcmp(cmd, "cd"))
		return (builtin_cd(exec->tools, args->argv));
	if (!ft_strcmp(cmd, "echo"))
		return (builtin_echo(exec->tools, args->argv));
	if (!ft_strcmp(cmd, "env"))
		return (builtin_env(exec->tools, args->argv));
	if (!ft_strcmp(cmd, "exit"))
		return (builtin_exit(exec->tools, args->argv));
	if (!ft_strcmp(cmd, "export"))
		return (builtin_export(exec->tools, args->argv));
	if (!ft_strcmp(cmd, "pwd"))
		return (builtin_pwd(exec->tools, args->argv));
	if (!ft_strcmp(cmd, "unset"))
		return (builtin_unset(exec->tools, args->argv));
	return (1);
}

int	execute_builtin(t_cmd_args *args, t_exec *exec)
{
	int	ret;

	if (!args || !args->argv[0])
		return (1);
	ret = execute_builtin_cmd(args->argv[0], args, exec);
	if (ret == -1)
		return (1);
	return (ret);
}

static t_cmd_args	*get_builtin_args(t_pip *cmd)
{
	if (!cmd)
		return (NULL);
	return (parse_command_args(cmd->redirection
			? cmd->redirection->cmd : cmd->cmd_pipe));
}

int	handle_builtin(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	int			ret;

	args = get_builtin_args(cmd);
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
