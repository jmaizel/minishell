/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:26:11 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 15:52:27 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

#define PATH_MAX 4096

/* Builtins */
int		builtin_env(t_tools *tools, char **argv);
int		builtin_export(t_tools *tools, char **argv);
int		builtin_unset(t_tools *tools, char **argv);
int		builtin_exit(t_tools *tools, char **argv);
int   builtin_env(t_tools *tools, char **argv);
int   builtin_cd(t_tools *tools, char **args);
int   builtin_echo(t_tools *tools, char **args);
int   builtin_pwd(t_tools *tools, char **args);

/* Environment utilities */
int		find_env_var(char **env, const char *name);
char	**update_env(char **env, const char *name, const char *value);
char	*get_env_name(const char *var);
char	*get_env_value(const char *var);
int		count_env_vars(char **env);

#endif