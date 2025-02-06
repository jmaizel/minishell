/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:31:48 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/06 15:06:56 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "../libft/includes/libft.h"
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

# define ERROR_EXIT 1
# define SUCCESS_EXIT 0
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define SUCCESS 0
# define GENERAL_ERROR 1

# define ERR_INVALID_CMD 1
# define ERR_MALLOC_FAILURE 2
# define ERR_GETCWD_FAILED 3
# define ERR_CHDIR_FAILED 4
# define ERR_EXEC_FAILURE 5

typedef struct s_executor
{
	t_tools	*tools;
}	t_executor;

typedef struct s_redirection_handler
{
	t_pip	*pip;
}	t_redirection_handler;

typedef struct s_builtin_handler
{
	t_executor	*executor;
}	t_builtin_handler;

typedef struct s_env_manager
{
	t_tools	*tools;
}	t_env_manager;

typedef struct s_pipe_manager
{
	int		*pipes;
	int		pipe_count;
}	t_pipe_manager;

typedef struct s_signal_handler
{
	void	(*sigint_handler)(int);
	void	(*child_handler)(int);
}	t_signal_handler;

typedef struct s_cleanup_manager
{
	t_tools	*tools;
}	t_cleanup_manager;

void	execute_simple_command(t_pip *pip, t_tools *tools,
			t_env_manager *env_mgr);
void	execute_external_command(t_pip *pip, t_tools *tools,
			t_env_manager *env_mgr);
void	update_exit_status(t_tools *tools, int status);
char	*find_executable(const char *command, t_env_manager *env_mgr);

void	apply_redirections(t_pip *pip, t_tools *tools);
void	handle_input_redirection(char *file);
void	handle_output_redirection(char *file);
void	handle_append_redirection(char *file);
void	handle_heredoc(char *delim, t_pip *pip, t_tools *tools);

char	*get_env_var(const char *key, t_env_manager *env_mgr);
int		add_env_var(char *var, t_env_manager *env_mgr);
int		remove_env_var(char *key, t_env_manager *env_mgr);
char	**duplicate_env(t_env_manager *env_mgr);
void	free_env(char **env);

int		count_pipes(t_pip *cmd);
int		*create_pipes(int count);
void	close_all_pipes(int *pipes, int pipe_count);
pid_t	*allocate_pids(int count);

void	setup_signals_exec(void);
void	sigint_handler(int sig);
void	child_sigint_handler(int sig);

void	free_str_array_exec(char **array);
void	cleanup_parsed_cmd(t_parsed_cmd *cmd);
void	cleanup_pip(t_pip *pip);
void	handle_error(const char *msg, t_tools *tools, int exit_code);
void	cleanup_executor(t_cleanup_manager *cleanup);
void	cleanup_env_manager(t_env_manager *env_mgr);
void	execute_pipeline(t_sep *cell, t_tools *tools, t_env_manager *env_mgr);
#endif