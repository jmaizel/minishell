/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 11:03:17 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void    cleanup_command(t_simple_cmds *cmd)
{
        t_lexer *redir;
        t_lexer *next;

        if (!cmd)
                return;

        if (cmd->str)
        {
                for (int i = 0; cmd->str[i]; i++)
                        free(cmd->str[i]);
                free(cmd->str);
        }

        if (cmd->hd_file_name)
                free(cmd->hd_file_name);

        redir = cmd->redirections;
        while (redir)
        {
                next = redir->next;
                if (redir->token.value)
                        free(redir->token.value);
                free(redir);
                redir = next;
        }
}