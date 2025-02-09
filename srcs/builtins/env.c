/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:20:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/09 18:21:09 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_env(t_tools *tools, char **argv)
{
    if (argv[1] != NULL)
    {
        ft_printf("env: no options or arguments are supported\n");
        return (1);
    }
    int i = 0;
    while (tools->env && tools->env[i])
    {
        ft_printf("%s\n", tools->env[i]);
        i++;
    }
    return (0);
}
