/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 16:54:14 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/28 11:31:51 by cdedessu         ###   ########.fr       */
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
	t_simple_cmds	cmd;
	char			**args;

	ft_memset(&cmd, 0, sizeof(t_simple_cmds));
	args = malloc(2 * sizeof(char *));
	if (!args)
	{
		ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
		return ;
	}
	args[0] = ft_strdup(line);
	if (!args[0])
	{
		free(args);
		ft_putendl_fd("Error: Memory allocation failed", STDERR_FILENO);
		return ;
	}
	args[1] = NULL;
	cmd.str = args;
	cmd.redirections = NULL;
	cmd.next = NULL;
	cmd.prev = NULL;
	execute_simple_command(&cmd, tools);
	free(args[0]);
	free(args);
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

	tmp = env;
	while (*tmp)
		free(*tmp++);
	free(env);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_tools	tools;

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

/* int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	t_tools tools;
	ft_memset(&tools, 0, sizeof(t_tools));
	tools.env = env;
	tools.exit_code = 0;
	tools.tokens = NULL;
	tools.cmds = NULL;
	loop_prompt(&tools, env);
	return (tools.exit_code);
} */