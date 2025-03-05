/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 01:30:47 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 14:11:01 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/minishell.h"

static void	print_sorted_env(t_tools *tools)
{
	int		i;
	char	**all_vars;
	int		total_count;

	all_vars = copy_env_and_exports(tools->env, &total_count, tools);
	if (!all_vars)
		return ;
	sort_env_array(all_vars);
	i = 0;
	while (i < total_count)
	{
		print_env_variable(all_vars[i]);
		free(all_vars[i]);
		i++;
	}
	free(all_vars);
}

static int	add_to_export_vars(const char *name, t_tools *tools)
{
	int	i;

	i = 0;
	while (i < tools->export_count)
	{
		if (ft_strcmp(tools->export_vars[i], name) == 0)
			return (0);
		i++;
	}
	if (tools->export_count < 100)
	{
		tools->export_vars[tools->export_count] = ft_strdup(name);
		tools->export_count++;
	}
	return (0);
}

static int	update_env_var2(t_tools *tools, char *name, char *value)
{
	char	**new_env;

	new_env = update_env(tools->env, name, value);
	tools->env = new_env;
	return (0);
}

static int	process_export_var(t_tools *tools, char *name, char *arg)
{
	char	*value;

	if (ft_strchr(arg, '='))
	{
		value = get_env_value(arg);
		if (value)
		{
			update_env_var2(tools, name, value);
			free(value);
		}
		else
			update_env_var(tools, name, "");
	}
	else
	{
		add_to_export_vars(name, tools);
	}
	return (0);
}

int	builtin_export(t_tools *tools, char **argv)
{
	int		i;
	char	*name;
	int		ret;

	ret = 0;
	if (!argv[1])
		return (print_sorted_env(tools), 0);
	i = 1;
	while (argv[i])
	{
		name = get_env_name(argv[i]);
		if (!name || !is_valid_identifier(name))
		{
			ft_printf("minishell: export: '%s': not a valid identifier\n",
				argv[i]);
			free(name);
			ret = 1;
			i++;
			continue ;
		}
		process_export_var(tools, name, argv[i]);
		free(name);
		i++;
	}
	return (ret);
}
