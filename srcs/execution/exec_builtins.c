/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:25:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/09 18:31:46 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
#include "../../includes/builtins.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	// Ajoutez d'autres builtins ici
	return (0);
}

int	execute_builtin(t_cmd_args *args, t_exec *exec)
{
	if (!args || !args->argv[0])
		return (1);

	if (ft_strcmp(args->argv[0], "env") == 0)
		return (builtin_env(exec->tools, args->argv));
	// Ajoutez d'autres builtins ici

	return (1);
}

int	handle_builtin(t_pip *cmd, t_exec *exec)
{
	t_cmd_args	*args;
	int			ret;

	args = parse_command_args(cmd->redirection ? cmd->redirection->cmd 
		: cmd->cmd_pipe);
	if (!args || !args->argv[0])
		return (1);
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
