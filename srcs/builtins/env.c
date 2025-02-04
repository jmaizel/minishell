/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:52:26 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 20:54:40 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	handle_env_errors(t_parsed_cmd *cmd, t_env_manager *env_mgr)
{
	char	**args;

	if (!cmd || !env_mgr || !env_mgr->tools || !env_mgr->tools->env)
		return (ERR_INVALID_CMD);
	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (ERR_MALLOC_FAILURE);
	if (args[1])
	{
		free_str_array(args);
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		return (ERR_INVALID_CMD);
	}
	free_str_array(args);
	if (!env_mgr->tools->env)
	{
		ft_putendl_fd("env: environment is empty", STDERR_FILENO);
		return (ERR_INVALID_CMD);
	}
	return (SUCCESS);
}

int	builtin_env(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
{
	int	i;
	int	ret;

	(void)tools;
	ret = handle_env_errors(cmd, env_mgr);
	if (ret != SUCCESS)
		return (ret);
	i = 0;
	while (env_mgr->tools->env[i])
	{
		ft_putendl_fd(env_mgr->tools->env[i], STDOUT_FILENO);
		i++;
	}
	return (SUCCESS);
}
