/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/17 17:02:01 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_child_signal(int sig)
{
	(void)sig;
	g_signal_received = 1;
}

static void	handle_parent_signal(int sig)
{
	(void)sig;
}

static void	setup_child_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_child_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	setup_parent_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_parent_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static char	*append_to_buffer(char *buffer, const char *line)
{
	char	*temp;
	char	*new_buffer;

	temp = ft_strjoin(buffer, line);
	if (!temp)
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_strjoin(temp, "\n");
	free(temp);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	free(buffer);
	return (new_buffer);
}

static int write_heredoc_contents(int fd, char **delimiters, int count)
{
    char    *line;
    char    *buffer;
    int     current_doc;

    buffer = ft_strdup("");
    if (!buffer)
        return (-1);
    
    current_doc = 0;
    while (current_doc < count && !g_signal_received)
    {
        line = readline("heredoc> ");
        if (!line || g_signal_received)
        {
            free(buffer);
            if (line)
                free(line);
            return (-1);
        }
        
        if (ft_strcmp(line, delimiters[current_doc]) == 0)
        {
            write(fd, buffer, ft_strlen(buffer));
            free(buffer);
            buffer = ft_strdup("");
            if (!buffer)
            {
                free(line);
                return (-1);
            }
            if (current_doc < count - 1) // Si ce n'est pas le dernier heredoc
                write(fd, "\n", 1);
            current_doc++;
        }
        else
        {
            char *new_buffer = append_to_buffer(buffer, line);
            if (!new_buffer)
            {
                free(line);
                free(buffer);
                return (-1);
            }
            buffer = new_buffer;
        }
        free(line);
    }
    
    int ret = (current_doc == count) ? 0 : -1;
    free(buffer);
    return (ret);
}

int handle_heredoc(char **delimiters, int count)
{
    int     pipe_fd[2];
    pid_t   pid;
    int     status;

    if (!delimiters || count <= 0)
        return (-1);
        
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
    {
        setup_child_heredoc_signals();
        close(pipe_fd[0]);
        status = write_heredoc_contents(pipe_fd[1], delimiters, count);
        close(pipe_fd[1]);
        if (g_signal_received)
            ft_putchar_fd('\n', STDERR_FILENO);
        _exit(status == -1 ? 1 : 0);
    }
    
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    g_signal_received = 0;
    setup_parent_signals();  // Utiliser setup_parent_signals au lieu de setup_interactive_signals
    
    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
        {
            close(pipe_fd[0]);
            g_signal_received = 1;
            return (-1);
        }
    }
    
    if (WEXITSTATUS(status) != 0)
    {
        close(pipe_fd[0]);
        return (-1);
    }
    
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    {
        close(pipe_fd[0]);
        return (-1);
    }
    
    close(pipe_fd[0]);
    return (0);
}