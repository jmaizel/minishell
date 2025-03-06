/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:11:17 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/06 13:39:30 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

# define PATH_MAX 4096

/* Builtins */
int		builtin_env(t_tools *tools, char **argv);
int		builtin_export(t_tools *tools, char **argv);
int		builtin_unset(t_tools *tools, char **argv);
int		builtin_exit(t_tools *tools, char **argv, int in_pipeline);
int		builtin_env(t_tools *tools, char **argv);
int		builtin_cd(t_tools *tools, char **args);
int		builtin_echo(t_tools *tools, char **args);
int		builtin_pwd(t_tools *tools, char **args);

/* Environment utilities */
int		find_env_var(char **env, const char *name);
char	**update_env(char **env, const char *name, const char *value);
char	*get_env_name(const char *var);
char	*get_env_value(const char *var);
int		count_env_vars(char **env);
char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3);
void	increment_shell_level(t_tools *tools);
char	*expand_home_path(char *path);
int		update_env_var(t_tools *tools, const char *name, const char *value);
int		count_env_vars(char **env);
int		find_env_var(char **env, const char *name);
char	*create_env_string_no_value(const char *name, size_t name_len);
char	*create_env_string(const char *name, const char *value);
char	**replace_env_var(char **env, int index, const char *name,
			const char *value);
int		is_valid_identifier(const char *str);
void	sort_env_array(char **sorted);
void	print_env_variable(char *env_var);
char	**copy_env_and_exports(char **env, int *total_count, t_tools *tools);
#endif