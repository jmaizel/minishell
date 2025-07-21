/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:08 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/10 18:01:25 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	setup_parent_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	process_intermediate_heredocs(t_parsed_cmd *cmd, t_exec *exec,
		int pipe_fd[2])
{
	int	i;
	int	temp_fd;

	if (cmd->heredoc_count <= 1)
		return ;
	i = 0;
	while (i < cmd->heredoc_count - 1 && !g_signal_received)
	{
		temp_fd = open("/dev/null", O_WRONLY);
		if (temp_fd == -1)
			exit(1);
		if (process_heredoc(temp_fd, cmd->heredoc_delim[i], exec->tools))
		{
			close(temp_fd);
			close(pipe_fd[1]);
			exit(1);
		}
		close(temp_fd);
		i++;
	}
}

static void	child_heredoc_process(t_parsed_cmd *cmd, t_exec *exec,
		int pipe_fd[2])
{
	setup_child_heredoc_signals();
	close(pipe_fd[0]);
	process_intermediate_heredocs(cmd, exec, pipe_fd);
	if (!g_signal_received)
	{
		if (process_heredoc(pipe_fd[1], cmd->heredoc_delim[cmd->heredoc_count
					- 1], exec->tools))
		{
			close(pipe_fd[1]);
			exit(1);
		}
	}
	close(pipe_fd[1]);
	exit(0);
}

int	handle_heredoc(t_parsed_cmd *cmd, t_exec *exec)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	g_signal_received = 0;
	setup_parent_heredoc_signals();
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
		child_heredoc_process(cmd, exec, pipe_fd);
	return (wait_heredoc_child(pid, pipe_fd, exec));
}
