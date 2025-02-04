/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:42:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 20:55:19 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../includes/minishell.h"
# include "../includes/execution.h"
# include "../includes/tools.h"

int	builtin_echo(t_parsed_cmd *cmd);
int	builtin_cd(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr);
int	builtin_pwd(t_parsed_cmd *cmd, t_tools *tools);
int	builtin_env(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr);	
int	builtin_export(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr);
int	builtin_unset(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr);
int	builtin_exit(t_parsed_cmd *cmd, t_tools *tools, t_env_manager *env_mgr);

# define BUILTIN_SUCCESS 0
# define BUILTIN_ERROR 1

#endif 