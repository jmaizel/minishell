/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:42:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/28 18:16:22 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../includes/minishell.h"
# include "../includes/execution.h"

int	builtin_echo(t_simple_cmds *cmd);
int	builtin_cd(t_simple_cmds *cmd, t_tools *tools);
int	builtin_pwd(t_simple_cmds *cmd, t_tools *tools);
int	builtin_env(t_simple_cmds *cmd, t_tools *tools);
int	builtin_export(t_simple_cmds *cmd, t_tools *tools);

#endif