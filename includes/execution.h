/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:31:48 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 15:54:23 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stddef.h>
# include <signal.h>
# include <stdbool.h>
# include "builtins.h"

/* Macros et codes de retour */
# define ERROR_EXIT 1
# define SUCCESS_EXIT 0
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define SUCCESS 0
# define GENERAL_ERROR 1

/* Exécution des commandes */
void    execute_simple_command(t_simple_cmds *cmd, t_tools *tools);
void    execute_pipeline(t_simple_cmds *cmd, t_tools *tools);
void    update_exit_status(t_tools *tools, int status);
int     get_command_exit_status(t_simple_cmds *cmd);

/* Gestion des redirections */
void    apply_redirections(t_simple_cmds *cmd);
void    handle_input_redirection(t_lexer *redir);
void    handle_output_redirection(t_lexer *redir);
void    handle_append_redirection(t_lexer *redir);
void    handle_heredoc(t_lexer *redir);

/* Gestion des variables d'environnement */
char    *get_env_var(char *key, char **env);
int     add_env_var(char *var, char ***env);
int     remove_env_var(char *key, char ***env);
char    *expand_variables(char *str, t_tools *tools);

/* Utilitaires */
char    *find_executable(char *command, char **env);
int     ft_strcmp(const char *s1, const char *s2);
void    handle_error(const char *msg);
int     cleanup_pipes(int prev_pipe, int pipe_fds[2], bool has_next);
void    cleanup_command(t_simple_cmds *cmd);
char    *ft_charjoin(char *str, char c);

/* Gestion des signaux */
void    sigint_handler(int sig);
void    child_sigint_handler(int sig);
void    setup_signals(void);
void    setup_child_signals(void);

/* Builtins */
int     builtin_echo(t_simple_cmds *cmd);
int     builtin_cd(t_simple_cmds *cmd, t_tools *tools);
int     builtin_pwd(void);
int     builtin_export(t_simple_cmds *cmd, t_tools *tools);
int     builtin_unset(t_simple_cmds *cmd, t_tools *tools);
int     builtin_env(t_tools *tools);
int     builtin_exit(t_simple_cmds *cmd);



#endif