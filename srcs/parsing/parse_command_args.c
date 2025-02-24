/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/24 14:32:39 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	skip_spaces(char *cmd_str, int i)
{
	while (cmd_str[i] && ft_isspace(cmd_str[i]))
		i++;
	return (i);
}

static int	handle_quotes(char *cmd_str, int *i, int *in_quotes,
		char *quote_type)
{
	if (!*in_quotes && (cmd_str[*i] == '"' || cmd_str[*i] == '\''))
	{
		*quote_type = cmd_str[*i];
		*in_quotes = 1;
		(*i)++;
		return (1);
	}
	if (*in_quotes && cmd_str[*i] == *quote_type)
	{
		*in_quotes = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	get_token_length(char *cmd_str, int i)
{
	int		start;
	int		in_quotes;
	char	quote_type;

	start = i;
	in_quotes = 0;
	while (cmd_str[i])
	{
		if (handle_quotes(cmd_str, &i, &in_quotes, &quote_type))
			continue ;
		if (!in_quotes && ft_isspace(cmd_str[i]))
			break ;
		i++;
	}
	return (i - start);
}

static int	fill_args(char **args, char *cmd_str, int i, int *j)
{
	int		len;
	int		start;
	char	*arg;

	start = i;
	len = get_token_length(cmd_str, i);
	arg = ft_substr(cmd_str, start, len);
	if (!arg)
		return (0);
	args[*j] = clean_quotes(arg);
	if (!args[*j])
	{
		free(arg);
		return (0);
	}
	(*j)++;
	return (start + len);
}

static t_cmd_args	*init_cmd_args(char *cmd_str, char ***args)
{
	t_cmd_args	*cmd_args;

	if (!cmd_str)
		return (NULL);
	cmd_args = malloc(sizeof(t_cmd_args));
	if (!cmd_args)
		return (NULL);
	*args = malloc(sizeof(char *) * (count_args(cmd_str) + 1));
	if (!*args)
	{
		free(cmd_args);
		return (NULL);
	}
	return (cmd_args);
}

static int	parse_tokens(char **args, char *cmd_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd_str[i])
	{
		i = skip_spaces(cmd_str, i);
		if (!cmd_str[i])
			break ;
		i = fill_args(args, cmd_str, i, &j);
		if (!i)
		{
			free_str_array(args);
			return (0);
		}
	}
	args[j] = NULL;
	return (j);
}

t_cmd_args	*parse_command_args(char *cmd_str)
{
	t_cmd_args	*cmd_args;
	char		**args;
	int			argc;

	cmd_args = init_cmd_args(cmd_str, &args);
	if (!cmd_args)
		return (NULL);
	argc = parse_tokens(args, cmd_str);
	if (!argc)
	{
		free(cmd_args);
		return (NULL);
	}
	cmd_args->argv = args;
	cmd_args->cmd = ft_strdup(args[0]);
	cmd_args->argc = argc;
	return (cmd_args);
}

void	print_cmd_args(t_cmd_args *cmd_args)
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
}

void	free_cmd_args(t_cmd_args *cmd_args)
{
	int i;

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