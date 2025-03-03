/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 01:30:47 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 16:55:23 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/minishell.h"

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

static void	print_sorted_env(char **env)
{
	int		i;
	char	**sorted;
	int		count;

	count = count_env_vars(env);
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return ;
	i = -1;
	while (env[++i])
		sorted[i] = ft_strdup(env[i]);
	sorted[i] = NULL;
	sort_env_array(sorted);
	i = -1;
	while (sorted[++i])
	{
		print_env_variable(sorted[i]);
		free(sorted[i]);
	}
	free(sorted);
}

static char	**append_export_var(char **env, const char *name)
{
	char	**new_env;
	int		count;
	int		i;

	count = count_env_vars(env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (env);
	i = 0;
	while (i < count)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = ft_strdup(name);
	if (!new_env[count])
	{
		free(new_env);
		return (env);
	}
	new_env[count + 1] = NULL;
	free(env);
	return (new_env);
}

static int	process_export_var(t_tools *tools, char *name, char *value,
		char *arg)
{
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
		if (find_env_var(tools->env, name) == -1)
		{
			new_env = append_export_var(tools->env, name);
			if (new_env)
				tools->env = new_env;
		}
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
	{
		print_sorted_env(tools->env);
		return (0);
	}
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
		process_export_var(tools, name, NULL, argv[i]);
		free(name);
		i++;
	}
	return (ret);
}
