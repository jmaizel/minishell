/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:45:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/06 13:27:28 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

int	builtin_unset(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
{
	char	**args;
	int		i;

	if (!cmd || !tools || !env_mgr)
		return (ERR_INVALID_CMD);
	args = ft_split(cmd->full_cmd, ' ');
	if (!args)
		return (ERR_MALLOC_FAILURE);
	if (!args[1])
	{
		ft_putendl_fd("unset: not enough arguments", STDERR_FILENO);
		free_str_array(args);
		return (ERR_INVALID_CMD);
	}
	i = 1;
	while (args[i])
	{
		remove_env_var(args[i], env_mgr);
		i++;
	}
	free_str_array(args);
	return (SUCCESS);
}
