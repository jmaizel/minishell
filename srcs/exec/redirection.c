/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:33:23 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 13:10:19 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void handle_redirection(t_parsed_cmd *cmd)
{
    int i;
    
    for (i = 0; i < cmd->input_count; i++)
        handle_input_redirection(cmd->input_file[i]);
    for (i = 0; i < cmd->output_count; i++)
        handle_output_redirection(cmd->output_file[i]);
    for (i = 0; i < cmd->append_count; i++)
        handle_append_redirection(cmd->append_file[i]);
    for (i = 0; i < cmd->heredoc_count; i++)
        handle_heredoc(cmd->heredoc_delim[i]);
}

void apply_redirections(t_pip *cmd)
{
    if (cmd->redirection)
        handle_redirection(cmd->redirection);
}

void handle_input_redirection(char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2 failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

void handle_output_redirection(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2 failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

void handle_append_redirection(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2 failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
}
