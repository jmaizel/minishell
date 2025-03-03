/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:13 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:36:50 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static char	*try_direct_path(char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_path_array(char *cmd, char **paths)
{
	char	*temp;
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
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
	char	*default_paths[] = {"/usr/bin", "/bin", NULL};

	if (!cmd)
		return (NULL);
	cmd_path = try_direct_path(cmd);
	if (cmd_path)
		return (cmd_path);
	cmd_path = search_in_path_array(cmd, cmd_paths);
	if (cmd_path)
		return (cmd_path);
	return (search_in_path_array(cmd, (char **)default_paths));
}
