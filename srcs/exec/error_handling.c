/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:49:47 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 10:53:01 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void    handle_error(const char *msg)
{
        perror(msg);
        exit(ERROR_EXIT);
}

void    cleanup_pipes(int prev_pipe, int pipe_fds[2], bool has_next)
{
        if (prev_pipe != -1)
                close(prev_pipe);
        if (has_next)
        {
                close(pipe_fds[1]);
                prev_pipe = pipe_fds[0];
        }
}