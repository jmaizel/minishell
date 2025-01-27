/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 11:41:52 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 20:10:52 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	*check_access(char *path, char *command)
{
	char	*full_path;
	char	*temp;

	full_path = ft_strjoin(path, "/");
	if (!full_path)
		return (NULL);
	temp = ft_strjoin(full_path, command);
	free(full_path);
	full_path = temp;
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_executable(char *command, char **env)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	if (!command || !env)
		return (NULL);
	path = get_env_var("PATH", env);
	if (!path || !*path)
	{
		ft_putstr_fd("Error: PATH not found in environment.\n", STDERR_FILENO);
		return (NULL);
	}
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = check_access(paths[i], command);
		if (full_path)
		{
			ft_free_split(paths);
			return (full_path);
		}
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
