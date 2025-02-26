/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 01:30:47 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/25 19:53:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtins.h"

static int is_valid_identifier(const char *str)
{
    int i;

    if (!str || !*str || ft_isdigit(*str))
        return (0);
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static void print_sorted_env(char **env)
{
    int i;
    int j;
    char **sorted;
    char *tmp;
    int count;

    count = count_env_vars(env);
    sorted = malloc(sizeof(char *) * (count + 1));
    if (!sorted)
        return;
    i = -1;
    while (env[++i])
        sorted[i] = ft_strdup(env[i]);
    sorted[i] = NULL;
    i = -1;
    while (sorted[++i] && sorted[i + 1])
    {
        j = i;
        while (sorted[++j])
        {
            if (ft_strcmp(sorted[i], sorted[j]) > 0)
            {
                tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    i = -1;
    while (sorted[++i])
    {
        ft_printf("declare -x %s\n", sorted[i]);
        free(sorted[i]);
    }
    free(sorted);
}

int builtin_export(t_tools *tools, char **argv)
{
    int i;
    char *name;
    char *value;
    int ret;

    ret = 0;
    if (!argv[1])
    {
        print_sorted_env(tools->env);
        return (0);
    }
    i = 1;
    while (argv[i])
    {
        name = get_env_name(argv[i]);
        if (!name)
        {
            i++;
            continue;
        }
        if (!is_valid_identifier(name))
        {
            ft_printf("minishell: export: '%s': not a valid identifier\n", argv[i]);
            free(name);
            ret = 1;
            i++;
            continue;
        }
        value = get_env_value(argv[i]);
        if (value)
        {
            char **new_env = update_env(tools->env, name, value);
            if (new_env != tools->env) // Si realloc a été fait
            {
                free(tools->env); // Libère l’ancien tableau
                tools->env = new_env;
            }
            free(value);
        }
        free(name);
        i++;
    }
    return (ret);
}
