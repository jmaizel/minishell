/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:05:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/30 10:21:04 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

int     builtin_pwd(t_simple_cmds *cmd, t_tools *tools)
{
        char    *pwd;

        (void)tools;

        // Vérifier s'il y a des arguments
        if (cmd->str[1] != NULL)
        {
                ft_putendl_fd("pwd: too many arguments", STDERR_FILENO);
                return (ERR_INVALID_CMD);
        }

        pwd = getcwd(NULL, 0);
        if (!pwd)
        {
                perror("pwd");
                return (ERR_GETCWD_FAILED);
        }

        ft_putendl_fd(pwd, STDOUT_FILENO);
        free(pwd);
        return (SUCCESS);
}
