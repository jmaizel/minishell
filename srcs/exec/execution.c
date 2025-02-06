/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:00:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/06 16:44:54 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/execution.h"

static int	is_builtin(const char *cmd)
{
	return (cmd && (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
			|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
			|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "exit") == 0));
}

static int	execute_builtin(t_parsed_cmd *cmd, t_tools *tools,
		t_env_manager *env_mgr)
{
	int		ret;
	char	*cmd_name;

	if (!cmd || !cmd->cmd)
		return (0);
	cmd_name = cmd->cmd;
	if (ft_strcmp(cmd_name, "echo") == 0)
		ret = builtin_echo(cmd);
	else if (ft_strcmp(cmd_name, "cd") == 0)
		ret = builtin_cd(cmd, tools, env_mgr);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		ret = builtin_pwd(cmd, tools);
	else if (ft_strcmp(cmd_name, "env") == 0)
		ret = builtin_env(cmd, tools, env_mgr);
	else if (ft_strcmp(cmd_name, "export") == 0)
		ret = builtin_export(cmd, tools, env_mgr);
	else if (ft_strcmp(cmd_name, "unset") == 0)
		ret = builtin_unset(cmd, tools, env_mgr);
	else if (ft_strcmp(cmd_name, "exit") == 0)
		ret = builtin_exit(cmd, tools, env_mgr);
	else
		return (0);
	tools->exit_code = ret;
	return (1);
}

static void	check_input_files(t_parsed_cmd *cmd, t_tools *tools)
{
	int	i;

	i = 0;
	while (i < cmd->input_count)
	{
		if (access(cmd->input_file[i], F_OK) == -1)
		{
			ft_putstr_fd(cmd->input_file[i], STDERR_FILENO);
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			tools->exit_code = 1;
		}
		i++;
	}
}

void	execute_external_command(t_pip *pip, t_tools *tools,
		t_env_manager *env_mgr)
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
	check_input_files(cmd, tools);
	if (tools->exit_code != 0)
		return ;
	if (!cmd->cmd || cmd->cmd[0] == '\0')
	{
		tools->exit_code = 0;
		return ;
	}
	args = parse_command_args(cmd->cmd);
	if (!args)
	{
		tools->exit_code = ERR_MALLOC_FAILURE;
		return ;
	}
	path = find_executable(args->cmd, env_mgr);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(args->cmd, STDERR_FILENO);
		free_cmd_args(args);
		tools->exit_code = CMD_NOT_FOUND;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		apply_redirections(pip, tools);
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

void	execute_simple_command(t_pip *pip, t_tools *tools,
		t_env_manager *env_mgr)
{
	t_parsed_cmd	*cmd;

	if (!pip || !pip->redirection || !tools || !env_mgr)
	{
		handle_error("Structure de commande invalide", tools, ERR_INVALID_CMD);
		return ;
	}
	cmd = pip->redirection;
	if (!cmd->cmd)
	{
		handle_error("Commande vide", tools, GENERAL_ERROR);
		return ;
	}
	// Vérifier si c'est une commande builtin
	if (is_builtin(cmd->cmd))
	{
		tools->exit_code = execute_builtin(cmd, tools, env_mgr);
	}
	else
	{
		// Commande externe
		execute_external_command(pip, tools, env_mgr);
	}
	printf("Executing command: %s\n", pip->redirection->cmd);
}
