/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:17:29 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 13:45:29 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static int	execute_builtin(t_simple_cmds *cmd, t_tools *tools)
{
	if (ft_strcmp(cmd->str[0], "echo") == 0)
		return (builtin_echo(cmd));
	else if (ft_strcmp(cmd->str[0], "cd") == 0)
		return (builtin_cd(cmd, tools));
	else if (ft_strcmp(cmd->str[0], "exit") == 0)
		return (builtin_exit(cmd));
	else if (ft_strcmp(cmd->str[0], "env") == 0)
		return (builtin_env(tools));
	return (0);
}

static void	execute_external_command(t_simple_cmds *cmd, t_tools *tools)
{
	char	*path;
	pid_t	pid;
	int		status;

	path = find_executable(cmd->str[0], tools->env);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd->str[0], STDERR_FILENO);
		tools->exit_code = 127;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		apply_redirections(cmd);
		if (execve(path, cmd->str, tools->env) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork failed");
	tools->exit_code = WEXITSTATUS(status);
	free(path);
}

void	execute_simple_command(t_simple_cmds *cmd, t_tools *tools)
{
	if (!cmd || !cmd->str || !cmd->str[0])
		return ;
	if (execute_builtin(cmd, tools))
		return ;
	execute_external_command(cmd, tools);
}
