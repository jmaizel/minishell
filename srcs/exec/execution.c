/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:00:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 20:53:20 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static int	is_builtin(const char *cmd)
{
	return (cmd && (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0));
}

static int	execute_builtin(t_parsed_cmd *cmd, t_tools *tools)
{
	int		ret;
	char	*cmd_name;

	if (!cmd || !cmd->cmd)
		return (0);
	cmd_name = cmd->cmd;
	if (ft_strcmp(cmd_name, "echo") == 0)
		ret = builtin_echo(cmd);
	else if (ft_strcmp(cmd_name, "cd") == 0)
		ret = builtin_cd(cmd, tools);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		ret = builtin_pwd(cmd, tools);
	else if (ft_strcmp(cmd_name, "env") == 0)
		ret = builtin_env(cmd, tools);
	else if (ft_strcmp(cmd_name, "export") == 0)
		ret = builtin_export(cmd, tools);
	else if (ft_strcmp(cmd_name, "unset") == 0)
		ret = builtin_unset(cmd, tools);
	else if (ft_strcmp(cmd_name, "exit") == 0)
		ret = builtin_exit(cmd, tools);
	else
		return (0);
	tools->exit_code = ret;
	return (1);
}

void	execute_external_command(t_pip *pip, t_tools *tools, t_env_manager *env_mgr)
{
	char			*path;
	pid_t			pid;
	int				status;
	t_parsed_cmd	*cmd;
	t_cmd_args		*args;

	if (!pip || !pip->redirection || !tools || !env_mgr)
	{
		ft_putstr_fd("Error: invalid command structure.\n", STDERR_FILENO);
		tools->exit_code = ERR_INVALID_CMD;
		return ;
	}
	cmd = pip->redirection;
	if (!cmd->cmd)
	{
		ft_putstr_fd("Error: empty command.\n", STDERR_FILENO);
		tools->exit_code = ERR_INVALID_CMD;
		return ;
	}
	path = find_executable(cmd->cmd, env_mgr);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd->cmd, STDERR_FILENO);
		tools->exit_code = CMD_NOT_FOUND;
		return ;
	}
	args = parse_command_args(cmd->full_cmd);
	if (!args)
		return (free(path), tools->exit_code = ERR_MALLOC_FAILURE);
	pid = fork();
	if (pid == 0)
	{
		apply_redirections(pip);
		if (execve(path, args->argv, env_mgr->tools->env) == -1)
		{
			perror("execve failed");
			free_cmd_args(args);
			free(path);
			exit(ERR_EXEC_FAILURE);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		tools->exit_code = WEXITSTATUS(status);
	}
	else
	{
		perror("fork failed");
		tools->exit_code = ERR_EXEC_FAILURE;
	}
	free_cmd_args(args);
	free(path);
}

void	execute_simple_command(t_pip *pip, t_tools *tools, t_env_manager *env_mgr)
{
	t_parsed_cmd	*cmd;

	if (!pip || !pip->redirection || !tools || !env_mgr)
	{
		ft_putstr_fd("Error: invalid command structure.\n", STDERR_FILENO);
		return ;
	}
	cmd = pip->redirection;
	if (!cmd->cmd)
	{
		ft_putstr_fd("Error: empty command.\n", STDERR_FILENO);
		return ;
	}
	if (is_builtin(cmd->cmd))
		execute_builtin(cmd, tools);
	else
		execute_external_command(pip, tools, env_mgr);
}
