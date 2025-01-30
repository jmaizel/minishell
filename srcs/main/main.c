/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/30 14:40:57 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	t_tools tools;
	ft_memset(&tools, 0, sizeof(t_tools));
	tools.env = env;
	tools.exit_code = 0;
	tools.tokens = NULL;
	tools.cmds = NULL;
	loop_prompt(&tools, env);
	return (tools.exit_code);
}