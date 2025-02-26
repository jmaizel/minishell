/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:04:22 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/26 08:48:17 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

static char *expand_home_path(char *path)
{
    char *home;

    if (!path || path[0] != '~')
        return (ft_strdup(path));
    home = getenv("HOME");
    if (!home)
        return (ft_strdup(path));
    if (path[1] == '\0')
        return (ft_strdup(home));
    else if (path[1] == '/')
        return (ft_strjoin(home, path + 1));
    return (ft_strdup(path));
}

static int update_env_var(t_tools *tools, const char *name, const char *value)
{
    char *new_var;
    int idx;

    new_var = ft_strjoin_three(name, "=", value);
    if (!new_var)
        return (0);
    idx = find_env_var(tools->env, name);
    if (idx != -1)
    {
        free(tools->env[idx]);
        tools->env[idx] = new_var;
    }
    else
    {
        tools->env = update_env(tools->env, name, value);
        if (!tools->env)
        {
            free(new_var);
            return (0);
        }
    }
    return (1);
}

static void update_pwd_vars(t_tools *tools, const char *old_pwd)
{
    char current_pwd[PATH_MAX];

    if (!getcwd(current_pwd, PATH_MAX))
        return;
    if (old_pwd)
        update_env_var(tools, "OLDPWD", old_pwd);
    update_env_var(tools, "PWD", current_pwd);
}

int builtin_cd(t_tools *tools, char **args)
{
    char *path;
    char *expanded_path = NULL;
    int ret;
    char old_pwd[PATH_MAX];
    int home_idx;

    if (!getcwd(old_pwd, PATH_MAX))
        return (1);
    if (!args[1])
    {
        home_idx = find_env_var(tools->env, "HOME");
        if (home_idx == -1)
            return (ft_printf("minishell: cd: HOME not set\n"), 1);
        path = tools->env[home_idx] + 5; // Passe "HOME=" (5 caractères)
        expanded_path = ft_strdup(path);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        int oldpwd_idx = find_env_var(tools->env, "OLDPWD");
        if (oldpwd_idx == -1)
            return (ft_printf("minishell: cd: OLDPWD not set\n"), 1);
        expanded_path = ft_strdup(tools->env[oldpwd_idx] + 7);
        if (expanded_path)
            ft_printf("%s\n", expanded_path);
    }
    else
        expanded_path = expand_home_path(args[1]);
    if (!expanded_path)
        return (1);
    ret = chdir(expanded_path);
    if (ret == -1)
    {
        ft_printf("minishell: cd: %s: No such file or directory\n", args[1]);
        free(expanded_path);
        return (1);
    }
    update_pwd_vars(tools, old_pwd);
    free(expanded_path);
    return (0);
}
