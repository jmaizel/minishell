/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/19 21:13:35 by cdedessu         ###   ########.fr       */
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

void	setup_child_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_child_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_parent_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_parent_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static char    *append_to_buffer(char *buffer, const char *line)
{
    char    *temp;
    char    *new_buffer;

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

int write_heredoc_content(int fd, char *delimiter)
{
    char    *line;
    char    *buffer;
    size_t  len;
    int     saved_stdout;

    buffer = ft_strdup("");
    if (!buffer)
        return (-1);

    // Sauvegarder la sortie standard originale
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        free(buffer);
        return (-1);
    }

    while (!g_signal_received)
    {
        // Rediriger la sortie standard vers stderr pour readline
        dup2(STDERR_FILENO, STDOUT_FILENO);
        
        line = readline("heredoc> ");
        
        // Restaurer la sortie standard originale
        dup2(saved_stdout, STDOUT_FILENO);
        
        if (!line || g_signal_received)
        {
            free(buffer);
            free(line);
            close(saved_stdout);
            return (-1);
        }
        
        if (ft_strcmp(line, delimiter) == 0)
        {
            len = ft_strlen(buffer);
            write(fd, buffer, len);
            free(line);
            free(buffer);
            close(saved_stdout);
            return (0);
        }
        
        buffer = append_to_buffer(buffer, line);
        free(line);
        if (!buffer)
        {
            close(saved_stdout);
            return (-1);
        }
    }
    close(saved_stdout);
    free(buffer);
    return (-1);
}

int handle_heredoc(char *delimiter)
{
    int     pipe_fd[2];
    pid_t   pid;
    int     status;

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
        if (write_heredoc_content(pipe_fd[1], delimiter) == -1)
        {
            close(pipe_fd[1]);
            exit(1);
        }
        close(pipe_fd[1]);
        exit(0);
    }
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    setup_interactive_signals();
    if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
    {
        close(pipe_fd[0]);
        return (-1);
    }
    return (pipe_fd[0]);
}
