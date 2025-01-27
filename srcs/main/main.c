/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/27 15:48:50 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>

int main(void)
{
    char *command = "echo '| outfile' | cat infile";
    char **pipes = ft_split_pipes(command, '|');

    for (int i = 0; pipes[i]; i++)
    {
        printf("Segment %d: %s\n", i, pipes[i]);
    }
    free_str_array(pipes);
    return (0);
}