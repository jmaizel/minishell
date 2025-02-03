/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:45:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 21:04:29 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	count_pipes(t_pip *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		if (cmd->next)
			count++;
		cmd = cmd->next;
	}
	return (count);
}

void	cleanup_pipe_array(int **pipes, int count)
{
	int	i;

	if (!pipes || !*pipes)
		return ;
	i = 0;
	while (i < count)
	{
		close((*pipes)[i * 2]);
		close((*pipes)[i * 2 + 1]);
		i++;
	}
	free(*pipes);
	*pipes = NULL;
}

int	*create_pipes(int count)
{
	int	*pipes;
	int	i;

	if (count <= 0)
		return (NULL);
	pipes = malloc(sizeof(int) * 2 * count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (pipe(pipes + (i * 2)) == -1)
		{
			perror("pipe failed");
			cleanup_pipe_array(&pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_all_pipes(int *pipes, int pipe_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipe_count * 2)
	{
		close(pipes[i]);
		i++;
	}
	free(pipes);
}

pid_t	*allocate_pids(int count)
{
	pid_t	*pids;

	if (count <= 0)
		return (NULL);
	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
	{
		perror("malloc failed for pids");
		return (NULL);
	}
	return (pids);
}
