/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/17 14:24:34 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_tools			tools;
	t_simple_cmds	cmd;

	(void)argc;
	(void)argv;
	tools.env = envp;
	tools.exit_code = 0;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		add_history(line);
	
		cmd.str = (char *[]){line, NULL};
		cmd.redirections = NULL;
		cmd.next = NULL;
		cmd.prev = NULL;

		apply_redirections(&cmd); 
		execute_simple_command(&cmd, &tools);

		free(line);
	}
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
