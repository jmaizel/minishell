/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:20:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 14:45:50 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
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

void	increment_shell_level(t_tools *tools)
{
	int		i;
	int		shlvl_index;
	int		shlvl_value;
	char	*new_value;
	int		minishell_exists;

	i = 0;
	shlvl_index = -1;
	shlvl_value = 0;
	minishell_exists = 0;
	while (tools->env[i])
	{
		if (ft_strncmp(tools->env[i], "MINISHELL=", 10) == 0)
		{
			minishell_exists = 1;
		}
		if (ft_strncmp(tools->env[i], "SHLVL=", 6) == 0)
		{
			shlvl_index = i;
			shlvl_value = ft_atoi(tools->env[i] + 6);
		}
		i++;
	}
	if (minishell_exists)
	{
		shlvl_value++;
	}
	else
	{
		shlvl_value = 1;
		tools->env = update_env(tools->env, "MINISHELL", "1");
	}
	new_value = ft_itoa(shlvl_value);
	if (shlvl_index != -1)
	{
		free(tools->env[shlvl_index]);
		tools->env[shlvl_index] = ft_strjoin("SHLVL=", new_value);
	}
	else
		tools->env = update_env(tools->env, "SHLVL", new_value);
	free(new_value);
}
