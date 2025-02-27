/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:43:13 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/26 09:00:42 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

char *get_cmd_path(char *cmd, char **cmd_paths)
{
    char *temp;
    char *cmd_path;
    int i;
    char *default_paths[] = {"/usr/bin", "/bin", NULL}; // Chemins de secours

    if (!cmd)
        return (NULL);
    if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    i = 0;
    while (cmd_paths && cmd_paths[i])
    {
        temp = ft_strjoin(cmd_paths[i], "/");
        cmd_path = ft_strjoin(temp, cmd);
        free(temp);
        if (access(cmd_path, X_OK) == 0)
            return (cmd_path);
        free(cmd_path);
        i++;
    }
    // Recherche dans les chemins par défaut si cmd_paths échoue
    i = 0;
    while (default_paths[i])
    {
        temp = ft_strjoin(default_paths[i], "/");
        cmd_path = ft_strjoin(temp, cmd);
        free(temp);
        if (access(cmd_path, X_OK) == 0)
            return (cmd_path);
        free(cmd_path);
        i++;
    }
    return (NULL);
}
