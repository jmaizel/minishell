/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/04 14:00:15 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	skip_spaces(char *cmd_str, int i)
{
	while (cmd_str[i] && ft_isspace(cmd_str[i]))
		i++;
	return (i);
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

static int	fill_args_list(char *cmd_str, char **args, t_cmd_args *cmd_args)
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
		i = extract_arg(args, cmd_str, i, &j);
		if (!i)
		{
			free_str_array(args);
			free(cmd_args);
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
	int			arg_count;

	cmd_args = init_cmd_args(cmd_str, &args);
	if (!cmd_args)
		return (NULL);
	arg_count = fill_args_list(cmd_str, args, cmd_args);
	if (!arg_count)
	{
		free(args);
		free(cmd_args);
		return (NULL);
	}
	cmd_args->argv = args;
	cmd_args->cmd = ft_strdup(args[0]);
	cmd_args->argc = arg_count;
	return (cmd_args);
}
