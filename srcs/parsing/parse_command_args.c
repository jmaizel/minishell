/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 11:55:31 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd_args	*parse_command_args(char *cmd_str)
{
	t_cmd_args	*cmd_args;

	cmd_args = malloc(sizeof(t_cmd_args));
	ft_memset(cmd_args, 0, sizeof(t_cmd_args));
	cmd_args->cmd = ft_strdup(cmd_str);
	cmd_args->argv = malloc(sizeof(char *) * 2);
	cmd_args->argv[0] = ft_strdup(cmd_str);
	cmd_args->argv[1] = NULL;
	cmd_args->argc = 1;
	return (cmd_args);
}

void	print_cmd_args(t_cmd_args *cmd_args)
{
	printf("Full Command: [%s]\n", cmd_args->cmd);
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