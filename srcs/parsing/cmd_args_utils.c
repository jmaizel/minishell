/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/27 14:27:57 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_token_length(char *cmd_str, int i)
{
	int		start;
	int		in_quotes;
	char	quote_type;

	start = i;
	in_quotes = 0;
	quote_type = 0;
	while (cmd_str[i])
	{
		if (!in_quotes && (cmd_str[i] == '"' || cmd_str[i] == '\''))
		{
			quote_type = cmd_str[i];
			in_quotes = 1;
		}
		else if (in_quotes && cmd_str[i] == quote_type)
			in_quotes = 0;
		else if (!in_quotes && ft_isspace(cmd_str[i]))
			break ;
		i++;
	}
	return (i - start);
}

int	extract_arg(char **args, char *cmd_str, int i, int *j)
{
	int		start;
	int		len;
	char	*arg;
	char	*cleaned;

	start = i;
	len = get_token_length(cmd_str, i);
	arg = ft_substr(cmd_str, start, len);
	if (!arg)
		return (0);
	cleaned = clean_quotes(arg);
	free(arg);
	if (!cleaned)
		return (0);
	args[*j] = cleaned;
	(*j)++;
	return (start + len);
}

/* void	print_cmd_args(t_cmd_args *cmd_args)
{
	int	i;

	if (!cmd_args)
		return ;
	printf("Command: [%s]\n", cmd_args->cmd);
	printf("Arguments:\n");
	i = 0;
	while (i < cmd_args->argc)
	{
		printf(" %d: [%s]\n", i, cmd_args->argv[i]);
		i++;
	}
} */

void	free_cmd_args(t_cmd_args *cmd_args)
{
	int	i;

	if (!cmd_args)
		return ;
	if (cmd_args->cmd)
		free(cmd_args->cmd);
	if (cmd_args->argv)
	{
		i = 0;
		while (i < cmd_args->argc)
		{
			if (cmd_args->argv[i])
				free(cmd_args->argv[i]);
			i++;
		}
		free(cmd_args->argv);
	}
	free(cmd_args);
}
