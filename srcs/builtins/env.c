/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:44:20 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 11:44:21 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	builtin_env(t_tools *tools)
{
	int	i;

	i = 0;
	while (tools->env[i])
	{
		ft_putendl_fd(tools->env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}