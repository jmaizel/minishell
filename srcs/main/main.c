/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/29 14:30:48 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    t_tools tools;
    
    // Initialiser la structure tools
    ft_memset(&tools, 0, sizeof(t_tools));
    
    // Copier les variables d'environnement
    tools.env = env;
    
    // Initialiser le code de sortie
    tools.exit_code = 0;
    
    // Initialiser les tokens et commandes à NULL
    tools.tokens = NULL;
    tools.cmds = NULL;
    
    // Lancer la boucle de prompt
    loop_prompt(&tools, env);
    
    return (tools.exit_code);
}