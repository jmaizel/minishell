/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:01:58 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 13:02:01 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int builtin_export(t_simple_cmds *cmd, t_tools *tools)
{
    // Implémentation minimale
    if (!cmd->str[1])
    {
        // Si aucun argument, afficher les variables d'environnement
        int i = 0;
        while (tools->env[i])
        {
            ft_putendl_fd(tools->env[i], STDOUT_FILENO);
            i++;
        }
        return (0);
    }
    
    // TODO: Ajouter la logique complète d'export
    return (0);
}