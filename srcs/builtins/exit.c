/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:53:02 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/02 19:38:37 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_numeric(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
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

static int	handle_exit_errors(char **args)
{
	if (args[1] && args[2])
	{
		free_str_array(args);
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (args[1] && !is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		free_str_array(args);
		exit(2);
	}
	return (0);
}

int	builtin_exit(t_parsed_cmd *cmd, t_tools *tools)
{
	int		exit_code;
	char	**args;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!cmd || !tools)
		return (1);
	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (ERR_MALLOC_FAILURE);
	if (handle_exit_errors(args))
		return (1);
	if (args[1])
	{
		exit_code = ft_atoi(args[1]);
		free_str_array(args);
	}
	else
	{
		free_str_array(args);
		exit_code = tools->exit_code;
	}
	exit(exit_code % 256);
}
