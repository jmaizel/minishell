/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 20:18:28 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/06 18:16:09 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_invalid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (1);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

static void	remove_env_var(char **env, int index)
{
	free(env[index]);
	while (env[index + 1])
	{
		env[index] = env[index + 1];
		index++;
	}
	env[index] = NULL;
}

static void	remove_from_export_vars(t_tools *tools, char *var_name)
{
	int	i;
	int	j;

	i = 0;
	while (i < tools->export_count)
	{
		if (ft_strcmp(tools->export_vars[i], var_name) == 0)
		{
			free(tools->export_vars[i]);
			j = i;
			while (j < tools->export_count - 1)
			{
				tools->export_vars[j] = tools->export_vars[j + 1];
				j++;
			}
			tools->export_vars[tools->export_count - 1] = NULL;
			tools->export_count--;
			return ;
		}
		i++;
	}
}

int	builtin_unset(t_tools *tools, char **argv)
{
	int	i;
	int	env_idx;

	i = 1;
	while (argv[i])
	{
		if (is_invalid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
		{
			env_idx = find_env_var(tools->env, argv[i]);
			if (env_idx >= 0)
				remove_env_var(tools->env, env_idx);
			remove_from_export_vars(tools, argv[i]);
		}
		i++;
	}
	return (0);
}
