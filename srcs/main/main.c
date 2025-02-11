/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:44:53 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/11 21:46:12 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/execution.h"

static void	init_minishell(t_tools *tools, char **env)
{
	ft_memset(tools, 0, sizeof(t_tools));
	tools->env = env;
	tools->exit_code = 0;
	tools->tokens = NULL;
	tools->cmds = NULL;
}

void	cleanup_minishell(t_tools *tools)
{
	(void)tools;
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
		parsing_line(user_input, tools);
		exec_commands(cell, tools);
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
	setup_parent_signals();

	while (1)
	{
		user_input = get_user_input();
		if (!user_input)
		{
			ft_printf("\nExit\n");
			break ;
		}
		if (user_input[0] != '\0')
			handle_execution(&tools, user_input);
		free(user_input);
	}

	cleanup_minishell(&tools);
	return (tools.exit_code);
}
