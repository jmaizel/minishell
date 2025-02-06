/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:05:04 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/06 16:57:29 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

/* static void	setup_pipe_fds(int input_fd, int *pipes, int i, t_pip *current)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input failed");
			exit(ERR_EXEC_FAILURE);
		}
		close(input_fd);
	}
	if (current->next)
	{
		if (dup2(pipes[i * 2 + 1], STDOUT_FILENO) == -1)
		{
			perror("dup2 output failed");
			exit(ERR_EXEC_FAILURE);
		}
	}
}

static void	close_pipe_fds(int *pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count * 2)
	{
		close(pipes[i]);
		i++;
	}
} */

/* static void	handle_child_process(t_pip *current, int input_fd, int *pipes,
		t_tools *tools, t_env_manager *env_mgr, int i, int pipe_count)
{
	setup_pipe_fds(input_fd, pipes, i, current);
	if (pipes)
		close_pipe_fds(pipes, pipe_count);
	setup_signals();
	apply_redirections(current, tools);
	if (current->redirection && current->redirection->cmd)
		execute_simple_command(current, tools, env_mgr);
	exit(tools->exit_code);
} */

/* static void	wait_for_children(pid_t *pids, int pipe_count, t_tools *tools)
{
	int	i;
	int	status;

	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], &status, 0);
		update_exit_status(tools, status);
		i++;
	}
} */
void    execute_pipeline(t_sep *cell, t_tools *tools, t_env_manager *env_mgr)
{
    int     pipe_count;
    int     (*pipes)[2];
    pid_t   *pids;
    t_pip   *current;
    int     i;
    int     j;

    pipe_count = count_pipes(cell->pipcell);
    pipes = malloc(sizeof(*pipes) * pipe_count);
    if (!pipes)
        return ;
    pids = malloc(sizeof(pid_t) * (pipe_count + 1));
    if (!pids)
    {
        free(pipes);
        return ;
    }
    current = cell->pipcell;
    i = 0;
    while (current)
    {
        if (current->next && pipe(pipes[i]) < 0)
        {
            free(pipes);
            free(pids);
            exit(1);
        }
        pids[i] = fork();
        if (pids[i] == -1)
        {
            free(pipes);
            free(pids);
            exit(1);
        }
        if (pids[i] == 0)
        {
            close(STDOUT_FILENO);  // Ferme stdout
               if (i > 0)
    {
        close(STDIN_FILENO);
        if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
            exit(1);
    }
    if (current->next)
    {
        close(STDOUT_FILENO);
        if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
            exit(1);
    }
            j = 0;
            while (j < pipe_count)
            {
                if (j != i)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                j++;
            }
            apply_redirections(current, tools);
            execute_simple_command(current, tools, env_mgr);
            exit(tools->exit_code);
        }
        if (i > 0)
            close(pipes[i - 1][0]);
        if (current->next)
            close(pipes[i][1]);
        current = current->next;
        i++;
    }
    i = 0;
    while (i <= pipe_count)
    {
        int status;
        waitpid(pids[i], &status, 0);
        update_exit_status(tools, status);
        i++;
    }
    free(pipes);
    free(pids);
}