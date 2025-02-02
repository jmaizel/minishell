/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:31:48 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/02 19:33:03 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "../libft/includes/libft.h"
# include "builtins.h"
# include "tools.h"
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

/* Error codes */
# define ERROR_EXIT 1
# define SUCCESS_EXIT 0
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define SUCCESS 0
# define GENERAL_ERROR 1

/* Error types */
# define ERR_INVALID_CMD 1
# define ERR_MALLOC_FAILURE 2
# define ERR_GETCWD_FAILED 3
# define ERR_CHDIR_FAILED 4
# define ERR_EXEC_FAILURE 5

/* Command execution */
void	execute_simple_command(t_pip *pip, t_tools *tools);
void	execute_pipeline(t_pip *pipeline, t_tools *tools);
void	execute_external_command(t_pip *pip, t_tools *tools);
void	update_exit_status(t_tools *tools, int status);
int		get_command_exit_status(t_pip *pip);

/* Redirection handling */
void	apply_redirections(t_pip *pip);
void	handle_input_redirection(char *file);
void	handle_output_redirection(char *file);
void	handle_append_redirection(char *file);
void	handle_heredoc(char *delim, t_pip *pip);

/* Environment variables */
char	*get_env_var(const char *key, char **env);
int		add_env_var(char *var, char ***env);
int		remove_env_var(char *key, char ***env);
char	**duplicate_env(char **env);
void	free_env(char **env);
char	*expand_variables(char *str, t_tools *tools);

/* Pipe handling */
int		count_pipes(t_pip *cmd);
void	cleanup_pipe_array(int **pipes, int count);
int		*create_pipes(int count);
void	close_all_pipes(int *pipes, int pipe_count);
pid_t	*allocate_pids(int count);

/* Utilities */
char	*find_executable(char *command, char **env);
int		ft_strcmp(const char *s1, const char *s2);
void	free_str_array_exec(char **array);
void	cleanup_parsed_cmd(t_parsed_cmd *cmd);
void	cleanup_pip(t_pip *pip);
void	cleanup_sep(t_sep *sep);
void	handle_error(const char *msg);

/* Signal handling */
void	sigint_handler(int sig);
void	child_sigint_handler(int sig);
void	setup_signals_exec(void);
void	setup_child_signals(void);

#endif