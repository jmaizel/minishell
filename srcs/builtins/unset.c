/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:02:23 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 13:05:35 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int builtin_unset(t_simple_cmds *cmd, t_tools *tools)
{
    int	i;

    if (!cmd->str[1])
    {
        ft_putendl_fd("Usage: unset VAR", STDERR_FILENO);
        return (1);
    }
    i = 1;
    while (cmd->str[i])
    {
        // Supprimez la variable d'environnement
        remove_env_var(cmd->str[i], &tools->env);
        i++;
    }
    return (0);
}