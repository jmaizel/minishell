/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:53 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/27 11:49:38 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/execution.h"
#include "../../includes/minishell.h"

int			g_signal_recieved = 0;

static void	init_minishell(t_tools *tools, char **env)
{
	int	i;
	int	j;

	ft_memset(tools, 0, sizeof(t_tools));
	i = 0;
	while (env[i])
		i++;
	tools->env = malloc(sizeof(char *) * (i + 1));
	if (!tools->env)
		return ;
	j = 0;
	while (j < i)
	{
		tools->env[j] = ft_strdup(env[j]);
		if (!tools->env[j])
		{
			while (--j >= 0)
				free(tools->env[j]);
			free(tools->env);
			tools->env = NULL;
			return ;
		}
		j++;
	}
	tools->env[i] = NULL;
	tools->exit_code = 0;
	tools->tokens = NULL;
	tools->cmds = NULL;
}

void	cleanup_minishell(t_tools *tools)
{
	int	i;

	if (tools->env)
	{
		i = 0;
		while (tools->env[i])
		{
			free(tools->env[i]);
			i++;
		}
		free(tools->env);
		tools->env = NULL;
	}
	rl_clear_history();
}

static int	handle_execution(t_tools *tools, char *user_input)
{
	t_sep	*cell;

	if (!user_input || check_invalid_chars(user_input))
	{
		ft_printf("Error: Invalid input\n");
		return (1);
	}
	cell = create_cell(ft_strdup(user_input));
	if (!cell)
		return (1);
	parse_pipes(cell);
	if (cell && cell->pipcell)
	{
		setup_exec_signals();
		parsing_line(user_input, tools);
		exec_commands(cell, tools);
		restore_signals();
	}
	free_cell(cell);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_tools	tools;
	char	*user_input;

	(void)argc;
	(void)argv;
	init_minishell(&tools, env);
	increment_shell_level(&tools);
	setup_interactive_signals();
	while (1)
	{
		g_signal_received = 0;
		user_input = get_user_input();
		if (!user_input || !ft_strncmp(user_input, "exit", 5))
		{
			if (user_input)
				free(user_input);
			break ;
		}
		if (user_input[0] != '\0')
			handle_execution(&tools, user_input);
		free(user_input);
	}
	cleanup_minishell(&tools);
	return (tools.exit_code);
}
