/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:42:12 by cberganz          #+#    #+#             */
/*   Updated: 2025/02/08 12:34:04 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

char	*get_cmd_path(char *cmd, char **cmd_paths)
{
	char	*temp;
	char	*cmd_path;
	int		i;

	if (!cmd || !cmd_paths)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (cmd_paths && cmd_paths[i])
	{
		temp = ft_strjoin(cmd_paths[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}
static int	prepare_command(t_pip *cmd, t_cmd_args **args, char **cmd_path,
	t_exec *exec)
{
	// Utiliser la commande sans redirections si disponible
	char	*cmd_str;

	cmd_str = cmd->redirection ? cmd->redirection->cmd : cmd->cmd_pipe;
	if (!cmd_str)
		return (1);

	*args = parse_command_args(cmd_str);
	if (!*args || !(*args)->argv[0])
	{
		if (*args)
			free_cmd_args(*args);
		return (1);
	}

	*cmd_path = get_cmd_path((*args)->argv[0], exec->cmd_paths);
	if (!*cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", (*args)->argv[0]);
		free_cmd_args(*args);
		return (127);
	}
	return (0);
}

static void	execute_child(t_pip *cmd, t_exec *exec, char *cmd_path,
	t_cmd_args *args)
{
	// Configurer les redirections avant d'exécuter la commande
	if (cmd->redirection)
	{
		if (setup_redirections(cmd->redirection, &exec->process) == -1)
		{
			free_cmd_args(args);
			free(cmd_path);
			exit(1);
		}
	}

	execve(cmd_path, args->argv, exec->tools->env);
	ft_printf("minishell: %s: command not found\n", args->argv[0]);
	free_cmd_args(args);
	free(cmd_path);
	exit(127);
}

int	exec_simple_cmd(t_pip *cmd, t_exec *exec)
{
	char		*cmd_path;
	t_cmd_args	*args;
	int			status;
	int			ret;

	if (!cmd)
		return (1);

	// Préparer la commande
	ret = prepare_command(cmd, &args, &cmd_path, exec);
	if (ret != 0)
		return (ret);

	// Créer un processus fils pour exécuter la commande
	exec->process.pid = fork();
	if (exec->process.pid == -1)
	{
		free(cmd_path);
		free_cmd_args(args);
		return (1);
	}

	if (exec->process.pid == 0)
		execute_child(cmd, exec, cmd_path, args);

	// Parent: attendre le fils et récupérer le statut
	waitpid(exec->process.pid, &status, 0);
	free(cmd_path);
	free_cmd_args(args);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
