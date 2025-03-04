/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 01:30:47 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 13:26:58 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/minishell.h"

char		*g_export_vars[100];
int			g_export_count = 0;

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || ft_isdigit(*str))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	sort_env_array(char **sorted)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (sorted[++i] && sorted[i + 1])
	{
		j = i;
		while (sorted[++j])
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
		}
	}
}

static void	print_env_variable(char *env_var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		ft_printf("declare -x %s=\"%s\"\n", env_var, equal_sign + 1);
		*equal_sign = '=';
	}
	else
	{
		ft_printf("declare -x %s\n", env_var);
	}
}

static char	**copy_env_and_exports(char **env, int *total_count)
{
	char	**all_vars;
	int		env_count;
	int		i;
	int		j;

	env_count = count_env_vars(env);
	*total_count = env_count + g_export_count;
	all_vars = malloc(sizeof(char *) * (*total_count + 1));
	if (!all_vars)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		all_vars[i] = ft_strdup(env[i]);
		i++;
	}
	j = 0;
	while (j < g_export_count)
	{
		all_vars[i + j] = ft_strdup(g_export_vars[j]);
		j++;
	}
	all_vars[*total_count] = NULL;
	return (all_vars);
}

static void	print_sorted_env(char **env)
{
	int		i;
	char	**all_vars;
	int		total_count;

	all_vars = copy_env_and_exports(env, &total_count);
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

static int	add_to_export_vars(const char *name)
{
	int	i;

	for (i = 0; i < g_export_count; i++)
	{
		if (ft_strcmp(g_export_vars[i], name) == 0)
			return (0);
	}
	if (g_export_count < 100)
	{
		g_export_vars[g_export_count] = ft_strdup(name);
		g_export_count++;
	}
	return (0);
}

static int	process_export_var(t_tools *tools, char *name, char *arg)
{
	char	*value;
	char	**new_env;

	if (ft_strchr(arg, '='))
	{
		value = get_env_value(arg);
		new_env = update_env(tools->env, name, value ? value : "");
		tools->env = new_env;
		free(value);
	}
	else
	{
		add_to_export_vars(name);
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
		return (print_sorted_env(tools->env), 0);
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
