/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/09 14:16:54 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd_args	*parse_command_args(char *cmd_str)
{
	t_cmd_args	*cmd_args;
	int			i;
	int			j;
	int			start;
	int			in_quotes;
	char		quote_type;
	char		**args;

	if (!cmd_str)
		return (NULL);
	cmd_args = malloc(sizeof(t_cmd_args));
	if (!cmd_args)
		return (NULL);
	args = malloc(sizeof(char *) * (count_args(cmd_str) + 1));
	if (!args)
	{
		free(cmd_args);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (cmd_str[i])
	{
		while (ft_isspace(cmd_str[i]))
			i++;
		if (!cmd_str[i])
			break ;
		start = i;
		in_quotes = 0;
		while (cmd_str[i])
		{
			if (!in_quotes && (cmd_str[i] == '"' || cmd_str[i] == '\''))
			{
				quote_type = cmd_str[i];
				in_quotes = 1;
				i++;
				continue ;
			}
			if (in_quotes && cmd_str[i] == quote_type)
			{
				in_quotes = 0;
				i++;
				continue ;
			}
			if (!in_quotes && ft_isspace(cmd_str[i]))
				break ;
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
	{
		return ;
	}

	if (cmd_args->cmd)
		free(cmd_args->cmd);

	if (cmd_args->argv)
	{
		int i = 0;
		while (i < cmd_args->argc)
		{
			if (cmd_args->argv[i])
			{
				free(cmd_args->argv[i]);
			}
			i++;
		}
		free(cmd_args->argv);
	}

	free(cmd_args);
}