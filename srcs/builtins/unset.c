/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:27:41 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/30 12:00:13 by cdedessu         ###   ########.fr       */
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

int	builtin_unset(t_simple_cmds *cmd, t_tools *tools)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->str || !tools || !tools->env)
		return (ERR_INVALID_CMD);
	if (!cmd->str[1])
		return (SUCCESS);
	i = 1;
	ret = SUCCESS;
	while (cmd->str[i])
	{
		if (!is_valid_identifier(cmd->str[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(cmd->str[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			ret = ERR_INVALID_CMD;
		}
		else
			remove_env_var(cmd->str[i], &tools->env);
		i++;
	}
	return (ret);
}
