/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 21:00:00 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

/* static char	*get_input(void)
{
	char			*line;
	char			**tmp;
	t_pip			*pip;
	t_parsed_cmd	*parsed_cmd;
	t_sep			*cell;
	t_pip			*current;

	line = readline("minishell> ");
	if (!line)
		ft_putendl_fd("exit", STDOUT_FILENO);
	else if (*line)
		add_history(line);
	return (line);
} */
/* static void	free_cmd_resources(t_pip *pip, t_parsed_cmd *parsed_cmd)
{
	if (parsed_cmd)
	{
		free(parsed_cmd->cmd);
		free(parsed_cmd->full_cmd);
		free(parsed_cmd);
	}
	if (pip)
		free(pip);
} */
/* static int	init_cmd_pipeline(t_pip **pip, t_parsed_cmd **parsed_cmd,
		char *line)
{
	*pip = malloc(sizeof(t_pip));
	if (!(*pip))
		return (ft_putendl_fd("Error: malloc failed for pipeline", 2), 0);
	ft_memset(*pip, 0, sizeof(t_pip));
	*parsed_cmd = malloc(sizeof(t_parsed_cmd));
	if (!(*parsed_cmd))
		return (free(*pip),
			ft_putendl_fd("Error: malloc failed for parsed command", 2), 0);
	ft_memset(*parsed_cmd, 0, sizeof(t_parsed_cmd));
	(*parsed_cmd)->cmd = ft_strdup(line);
	tmp = ft_split(line, ' ');
	if (!(*parsed_cmd)->cmd || !tmp)
	{
		free_cmd_resources(*pip, *parsed_cmd);
		ft_putendl_fd("Error: malloc failed", 2);
		return (0);
	}
	(*parsed_cmd)->full_cmd = tmp[0]; // Stocker seulement la première commande
	free(tmp); // Libérer le tableau
	(*pip)->redirection = *parsed_cmd;
	return (1);
} */
/* static void	execute_command_line(char *line, t_tools *tools,
		t_env_manager *env_mgr)
{
	if (!init_cmd_pipeline(&pip, &parsed_cmd, line))
		return ;
	execute_simple_command(pip, tools, env_mgr);
	free_cmd_resources(pip, parsed_cmd);
} */
void parse_and_execute(char *user_input, t_tools *tools, t_env_manager *env_mgr)
{
    t_sep *cell;
    t_pip *current;

    if (!user_input || check_invalid_chars(user_input))
	{
        ft_printf("Error: Invalid input\n");
		return ;
	}
    cell = create_cell(ft_strdup(user_input));
    if (!cell)
        return ;
    parse_pipes(cell);
    if (count_pipes(cell->pipcell) > 0)
        execute_pipeline(cell, tools, env_mgr);
    else
    {
        current = cell->pipcell;
        if (current && current->cmd_pipe)
        {
            current->redirection = parse_redir(current->cmd_pipe);
            if (current->redirection)
                execute_simple_command(current, tools, env_mgr);
        }
    }
    free_cell(cell);
}

char	**copy_env(char **envp)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = -1;
	while (++i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (i > 0)
				free(new_env[--i]);
			free(new_env);
			return (NULL);
		}
	}
	new_env[count] = NULL;
	return (new_env);
}

void	cleanup_env_manager(t_env_manager *env_mgr)
{
	if (env_mgr)
		free_env(env_mgr->tools->env);
}

int	main(int argc, char **argv, char **envp)
{
	t_tools			tools;
	t_env_manager	env_mgr;
	char			*user_input;
	char			**paths;

	(void)argc;
	(void)argv;
	ft_memset(&tools, 0, sizeof(t_tools));
	ft_memset(&env_mgr, 0, sizeof(t_env_manager));
	tools.env = copy_env(envp);
	env_mgr.tools = &tools;
	paths = get_env_paths(envp, "PATH");
	if (!paths)
	{
		ft_printf("Erreur : PATH non trouvé.\n");
		return (1);
	}
	setup_signals();
	while (1)
	{
		user_input = get_user_input();
		if (!user_input)
		{
			ft_printf("\nExit\n");
			break ;
		}
		if (user_input[0] == '\0')
		{
			free(user_input);
			continue ;
		}
		parse_and_execute(user_input, &tools, &env_mgr);
		free(user_input);
	}
	cleanup_executor(((t_cleanup_manager *)(&env_mgr)));
	free_str_array(paths);
	rl_clear_history();
	return (tools.exit_code);
}
