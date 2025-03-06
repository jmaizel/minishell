/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:46:53 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/05 14:11:12 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	is_valid_identifier(const char *str)
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

void	sort_env_array(char **sorted)
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

void	print_env_variable(char *env_var)
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

char	**copy_env_and_exports(char **env, int *total_count, t_tools *tools)
{
	char	**all_vars;
	int		env_count;
	int		i;
	int		j;

	env_count = count_env_vars(env);
	*total_count = env_count + tools->export_count;
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
	while (j < tools->export_count)
	{
		all_vars[i + j] = ft_strdup(tools->export_vars[j]);
		j++;
	}
	all_vars[*total_count] = NULL;
	return (all_vars);
}
