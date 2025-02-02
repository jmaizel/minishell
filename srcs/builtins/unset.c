/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:27:41 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/02 19:42:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_valid_identifier(const char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	i = 1;
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	process_unset_args(char **args, t_tools *tools)
{
	int	i;
	int	ret;

	i = 1;
	ret = SUCCESS;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			ret = ERR_INVALID_CMD;
		}
		else
			remove_env_var(args[i], &tools->env);
		i++;
	}
	return (ret);
}

int	builtin_unset(t_parsed_cmd *cmd, t_tools *tools)
{
	char	**args;
	int		ret;

	if (!cmd || !tools || !tools->env)
		return (ERR_INVALID_CMD);
	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (ERR_MALLOC_FAILURE);
	if (!args[1])
	{
		free_str_array(args);
		return (SUCCESS);
	}
	ret = process_unset_args(args, tools);
	free_str_array(args);
	return (ret);
}
