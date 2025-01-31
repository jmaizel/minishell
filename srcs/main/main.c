/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 13:32:37 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static char	*get_input(void)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
		ft_putendl_fd("exit", STDOUT_FILENO);
	else if (*line)
		add_history(line);
	return (line);
}

static void	execute_command_line(char *line, t_tools *tools)
{
	t_pip			*pip;
	t_parsed_cmd	*cmd;

	pip = malloc(sizeof(t_pip));
	if (!pip)
	{
		ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
		return ;
	}
	cmd = malloc(sizeof(t_parsed_cmd));
	if (!cmd)
	{
		free(pip);
		ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
		return ;
	}
	ft_memset(pip, 0, sizeof(t_pip));
	ft_memset(cmd, 0, sizeof(t_parsed_cmd));
	cmd->full_cmd = ft_strdup(line);
	cmd->cmd = ft_strdup(line);
	if (!cmd->full_cmd || !cmd->cmd)
	{
		cleanup_parsed_cmd(cmd);
		free(pip);
		ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
		return ;
	}
	pip->redirection = cmd;
	pip->next = NULL;
	pip->prev = NULL;
	execute_simple_command(pip, tools);
	cleanup_pip(pip);
}

char	**copy_env(char **envp)
{
	int		i;
	int		count;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

static void	cleanup_env(char **env)
{
	char	**tmp;

	if (!env)
		return ;
	tmp = env;
	while (*tmp)
		free(*tmp++);
	free(env);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_tools		tools;

	(void)argc;
	(void)argv;
	ft_memset(&tools, 0, sizeof(t_tools));
	tools.env = copy_env(envp);
	if (!tools.env)
	{
		ft_putendl_fd("Error: Failed to copy environment", STDERR_FILENO);
		return (1);
	}
	setup_signals();
	while (1)
	{
		line = get_input();
		if (!line)
			break ;
		execute_command_line(line, &tools);
		free(line);
	}
	cleanup_env(tools.env);
	return (tools.exit_code);
}

/* int main(void)
{
	char *test1 = "echo \"Hello 'world'\""; // Toutes les quotes sont fermées
	char *test2 = "echo 'Hello world";      // Single quote non fermée
	char *test3 = "echo \"Hello world";     // Double quote non fermée

	printf("Test 1: %d\n", is_quote_closed(test1)); // 1
	printf("Test 2: %d\n", is_quote_closed(test2)); // 0
	printf("Test 3: %d\n", is_quote_closed(test3)); // 0

	return (0);
} */
