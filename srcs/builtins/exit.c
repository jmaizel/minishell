/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:10:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 21:10:17 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
{
	int		exit_code;
	char	**args;

	ft_putendl_fd("exit", STDOUT_FILENO);
	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
	{
		exit(GENERAL_ERROR);
	}
	if (args[1] != NULL && !is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		free_str_array(args);
		exit(255);
	}
	if (args[1] != NULL && args[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		free_str_array(args);
		return (GENERAL_ERROR);
	}
	if (args[1] != NULL)
	{
		exit_code = ft_atoi(args[1]);
	}
	else
	{
		exit_code = tools->exit_code;
	}
	free_str_array(args);
	free_env(env_mgr->tools->env);
	exit(exit_code);
}
