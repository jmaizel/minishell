/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:26:11 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/12 13:52:07 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
 #define BUILTINS_H

# include "minishell.h"

int	builtin_env(t_tools *tools, char **argv);
int	builtin_cd(t_tools *tools, char **args);
int	builtin_echo(t_tools *tools, char **args);

#endif