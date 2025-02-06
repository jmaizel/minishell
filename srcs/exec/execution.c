/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:00:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/06 20:27:10 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include "../includes/builtins.h"

/**
 * Détermine si une commande donnée est un builtin.
 */
static int	is_builtin(const char *cmd)
{
	return (cmd && (
		ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0));
}

/**
 * Exécute la commande builtin correspondante (cd, echo, pwd, etc.).
 * @param cmd      Structure de commande parsée (t_parsed_cmd).
 * @param tools    Contexte global (environnement, exit_code, etc.).
 * @param env_mgr  Gestionnaire d'environnement (pour export, unset, etc.).
 * @return         1 si c'était un builtin reconnu, 0 sinon.
 */
static int	execute_builtin(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
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

/**
 * Exécute une commande externe (non-builtin).
 * @param pip      Contient la commande (dans pip->redirection) et ses redirections.
 * @param tools    Contexte global.
 * @param env_mgr  Gestion de l'environnement.
 */
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
		return;
	}
	cmd = pip->redirection;
	args = parse_command_args(cmd->cmd); // On parse la ligne de commande

	if (!args)
	{
		tools->exit_code = ERR_MALLOC_FAILURE;
		return;
	}
	path = find_executable(args->cmd, env_mgr);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(args->cmd, STDERR_FILENO);
		free_cmd_args(args);
		tools->exit_code = CMD_NOT_FOUND;
		return;
	}

	pid = fork();
	if (pid == 0) // Processus enfant
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
	else if (pid > 0) // Processus parent
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

/**
 * Exécute une commande simple (un seul t_pip), 
 * détecte si c'est un builtin ou une commande externe.
 */
void	execute_simple_command(t_pip *pip, t_tools *tools, t_env_manager *env_mgr)
{
	t_parsed_cmd	*cmd;

	if (!pip || !pip->redirection || !tools || !env_mgr)
	{
		ft_putendl_fd("Invalid command structure", STDERR_FILENO);
		return;
	}
	cmd = pip->redirection;
	if (!cmd->cmd)
	{
		ft_putendl_fd("Empty command", STDERR_FILENO);
		return;
	}
	// Si c'est un builtin, on l'exécute dans le processus parent
	if (is_builtin(cmd->cmd))
		execute_builtin(cmd, tools, env_mgr);
	else
		// Sinon, c'est un externe => fork + execve
		execute_external_command(pip, tools, env_mgr);
}
