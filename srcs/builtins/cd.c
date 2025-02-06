/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:31:22 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/06 20:31:24 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

int builtin_cd(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr)
{
    // Si 'tools' n'est pas utilisé du tout dans ta fonction, ajoute (void)tools
    (void)tools;

    char **args;
    char *oldpwd = NULL;
    char *newpwd = NULL;
    char *tmp_str = NULL;
    int  ret = BUILTIN_SUCCESS;

    // Récupération des arguments : "cd [arg]"
    args = ft_split(cmd->full_cmd, ' '); 
    if (!args)
        return (BUILTIN_ERROR);

    // On récupère le PWD courant (pour l'OLDPWD)
    oldpwd = getcwd(NULL, 0);

    // 1) Si pas d'argument -> aller dans $HOME
    if (!args[1])
    {
        char *home = get_env_var("HOME", env_mgr);
        if (!home)
        {
            ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
            free_str_array(args);
            free(oldpwd);
            return (BUILTIN_ERROR);
        }
        if (chdir(home) == -1)
        {
            ft_putstr_fd("cd: ", STDERR_FILENO);
            ft_putstr_fd(home, STDERR_FILENO);
            ft_putstr_fd(": ", STDERR_FILENO);
            ft_putendl_fd(strerror(errno), STDERR_FILENO);
            ret = BUILTIN_ERROR;
        }
    }
    // 2) Sinon -> cd <args[1]>
    else
    {
        if (chdir(args[1]) == -1)
        {
            ft_putstr_fd("cd: ", STDERR_FILENO);
            ft_putstr_fd(args[1], STDERR_FILENO);
            ft_putstr_fd(": ", STDERR_FILENO);
            ft_putendl_fd(strerror(errno), STDERR_FILENO);
            ret = BUILTIN_ERROR;
        }
    }

    // Si chdir a réussi, mettre à jour OLD_PWD et PWD
    if (ret == BUILTIN_SUCCESS)
    {
        // OLD_PWD = l'ancienne valeur de PWD (stockée dans 'oldpwd')
        if (oldpwd)
        {
            tmp_str = ft_strjoin("OLDPWD=", oldpwd);
            if (tmp_str)
            {
                add_env_var(tmp_str, env_mgr);
                free(tmp_str);
            }
        }
        // PWD = le nouveau répertoire
        newpwd = getcwd(NULL, 0);
        if (newpwd)
        {
            tmp_str = ft_strjoin("PWD=", newpwd);
            if (tmp_str)
            {
                add_env_var(tmp_str, env_mgr);
                free(tmp_str);
            }
            free(newpwd);
        }
    }

    free_str_array(args);
    free(oldpwd);
    return (ret);
}
