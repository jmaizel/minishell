/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/05 16:22:48 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd_args  *parse_command_args(char *cmd_str)
{
        t_cmd_args      *cmd_args;
        int             arg_count;
        char            **args;
        int             i;
        int             j;
        int             start;
        int             in_quotes;
        char            quote_type;

        cmd_args = malloc(sizeof(t_cmd_args));
        arg_count = count_args(cmd_str);
        args = malloc(sizeof(char *) * (arg_count + 1));
        i = 0;
        j = 0;
        in_quotes = 0;
        while (cmd_str[i])
        {
                while (ft_isspace(cmd_str[i]))
                        i++;
                start = i;
                while (cmd_str[i] && (in_quotes || !ft_isspace(cmd_str[i])))
                {
                        if (!in_quotes && (cmd_str[i] == '"' || cmd_str[i] == '\''))
                        {
                                quote_type = cmd_str[i];
                                in_quotes = 1;
                        }
                        else if (in_quotes && cmd_str[i] == quote_type)
                                in_quotes = 0;
                        i++;
                }
                args[j++] = clean_quotes(ft_substr(cmd_str, start, i - start));
        }
        args[j] = NULL;
        cmd_args->argv = args;
        cmd_args->cmd = ft_strdup(args[0]);
        cmd_args->argc = j;
        return (cmd_args);
}

void	print_cmd_args(t_cmd_args *cmd_args)
{
	int	i;

	printf("Command: [%s]\n", cmd_args->cmd);
	printf("Arguments:\n");
	i = 0;
	while (i < cmd_args->argc)
	{
		printf(" %d: [%s]\n", i, cmd_args->argv[i]);
		i++;
	}
}

void	free_cmd_args(t_cmd_args *cmd_args)
{
	if (!cmd_args)
		return ;

	free(cmd_args->cmd);
	free(cmd_args->argv[0]);
	free(cmd_args->argv);
	free(cmd_args);
}