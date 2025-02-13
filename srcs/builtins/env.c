/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:20:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/10 20:06:47 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_env(t_tools *tools, char **argv)
{
	int	i;

	if (argv[1] != NULL)
	{
		ft_putstr_fd("env: no options or arguments are supported\n", 2);
		return (1);
	}
	i = 0;
	while (tools->env && tools->env[i])
	{
		ft_putendl_fd(tools->env[i], 1);
		i++;
	}
	return (0);
}
