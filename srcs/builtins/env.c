/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:20:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/13 11:27:03 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

static void	print_and_free_env(char **sorted_env, int env_count)
{
	int	i;

	i = 0;
	while (i < env_count)
	{
		ft_putendl_fd(sorted_env[i], 1);
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

int	builtin_env(t_tools *tools, char **argv)
{
	char	**sorted_env;
	int		env_count;
	int		i;

	if (argv[1] != NULL)
	{
		ft_putstr_fd("env: no options or arguments are supported\n", 2);
		return (1);
	}
	env_count = copy_and_sort_env(tools, &sorted_env);
	if (env_count == 0)
	{
		i = 0;
		while (tools->env && tools->env[i])
		{
			ft_putendl_fd(tools->env[i], 1);
			i++;
		}
		return (0);
	}
	print_and_free_env(sorted_env, env_count);
	return (0);
}

static int	check_shell_vars(t_tools *tools, int *shlvl_index, int *shlvl_value,
		int *minishell_exists)
{
	int	i;

	i = 0;
	*shlvl_index = -1;
	*shlvl_value = 0;
	*minishell_exists = 0;
	while (tools->env[i])
	{
		if (ft_strncmp(tools->env[i], "MINISHELL=", 10) == 0)
			*minishell_exists = 1;
		if (ft_strncmp(tools->env[i], "SHLVL=", 6) == 0)
		{
			*shlvl_index = i;
			*shlvl_value = ft_atoi(tools->env[i] + 6);
		}
		i++;
	}
	return (i);
}

static void	update_shell_level(t_tools *tools, int shlvl_index, int shlvl_value)
{
	char	*new_value;

	new_value = ft_itoa(shlvl_value);
	if (!new_value)
		return ;
	if (shlvl_index != -1)
	{
		free(tools->env[shlvl_index]);
		tools->env[shlvl_index] = ft_strjoin("SHLVL=", new_value);
	}
	else
		tools->env = update_env(tools->env, "SHLVL", new_value);
	free(new_value);
}

void	increment_shell_level(t_tools *tools)
{
	int	shlvl_index;
	int	shlvl_value;
	int	minishell_exists;

	check_shell_vars(tools, &shlvl_index, &shlvl_value, &minishell_exists);
	if (minishell_exists)
		shlvl_value++;
	else
	{
		shlvl_value = 1;
		tools->env = update_env(tools->env, "MINISHELL", "1");
	}
	update_shell_level(tools, shlvl_index, shlvl_value);
}
