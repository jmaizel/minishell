/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:13 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:20:31 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static char	*try_default_paths(char *cmd)
{
	char	*temp;
	char	*cmd_path;
	int		i;
	char	*default_paths[] = {"/usr/bin", "/bin", NULL};

	i = 0;
	while (default_paths[i])
	{
		temp = ft_strjoin(default_paths[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

static char	*try_cmd_paths(char *cmd, char **cmd_paths)
{
	char	*temp;
	char	*cmd_path;
	int		i;

	i = 0;
	while (cmd_paths && cmd_paths[i])
	{
		temp = ft_strjoin(cmd_paths[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **cmd_paths)
{
	char	*cmd_path;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	cmd_path = try_cmd_paths(cmd, cmd_paths);
	if (cmd_path)
		return (cmd_path);
	return (try_default_paths(cmd));
}
