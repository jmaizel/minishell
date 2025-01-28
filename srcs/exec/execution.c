/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:17:29 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/28 14:20:18 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static int	is_builtin(char *cmd)
{
	return (cmd && (ft_strcmp(cmd, "echo") == 0
			|| ft_strcmp(cmd, "cd") == 0
			|| ft_strcmp(cmd, "pwd") == 0));
}

static int	execute_builtin(t_simple_cmds *cmd, t_tools *tools)
{
	int	ret;

	if (ft_strcmp(cmd->str[0], "echo") == 0)
		ret = builtin_echo(cmd);
	else if (ft_strcmp(cmd->str[0], "cd") == 0)
		ret = builtin_cd(cmd, tools);
	else if (ft_strcmp(cmd->str[0], "pwd") == 0)
		ret = builtin_pwd(cmd, tools);
	else
		return (0);
	tools->exit_code = ret;
	return (1);
}

void	execute_external_command(t_simple_cmds *cmd, t_tools *tools)
{
	char	*path;
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->str || !cmd->str[0] || !tools)
	{
		ft_putstr_fd("Error: invalid command structure.\n", STDERR_FILENO);
		tools->exit_code = ERR_INVALID_CMD;
		return ;
	}
	path = find_executable(cmd->str[0], tools->env);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd->str[0], STDERR_FILENO);
		tools->exit_code = CMD_NOT_FOUND;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		apply_redirections(cmd);
		if (execve(path, cmd->str, tools->env) == -1)
		{
			perror("execve failed");
			exit(ERR_EXEC_FAILURE);
		}
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork failed");
	free(path);
}

void	execute_simple_command(t_simple_cmds *cmd, t_tools *tools)
{
	if (!cmd || !cmd->str || !cmd->str[0] || !tools)
	{
		ft_putstr_fd("Error: invalid command structure.\n", STDERR_FILENO);
		return ;
	}
	if (is_builtin(cmd->str[0]))
		execute_builtin(cmd, tools);
	else
		execute_external_command(cmd, tools);
}
