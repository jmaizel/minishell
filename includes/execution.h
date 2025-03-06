/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:22 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/05 15:57:49 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>

# define PIPE_READ 0
# define PIPE_WRITE 1

typedef struct s_process
{
	pid_t		pid;
	int			pipe_fd[2];
	int			stdin_backup;
	int			stdout_backup;
}				t_process;

typedef struct s_exec
{
	t_tools		*tools;
	t_process	process;
	int			exit_status;
	int			pipe_count;
	char		**cmd_paths;
}				t_exec;

typedef struct s_exec_params
{
	t_pip		*cmd;
	t_exec		*exec;
	int			i;
	int			pipes[1024][2];
	int			heredoc_fd;
}				t_exec_params;

typedef struct s_fork_params
{
	t_pip		*cmd;
	t_exec		*exec;
	int			i;
	int			pipes[1024][2];
	int			heredoc_fd;
	pid_t		*pids;
}				t_fork_params;

/* exec.c */
void			init_exec_struct(t_exec *exec, t_tools *tools);
int				exec_commands(t_sep *cell, t_tools *tools);

/* exec_expansion.c */
void			expand_cmd_and_check_redir(t_pip *current, t_tools *tools);
void			expand_commands(t_pip *current, t_tools *tools);
int				expand_and_check_redirection(t_parsed_cmd *redir, t_exec *exec);

/* exec_helper.c */
t_parsed_cmd	*find_redir(t_pip *current);
int				manage_heredoc(t_parsed_cmd *redir, t_exec *exec,
					int *heredoc_fd);
int				prepare_heredoc_fd(t_exec *exec, int *heredoc_fd);
int				handle_heredoc_and_expand(t_sep *cell, t_exec *exec,
					int *heredoc_fd);

/* exec_cmd.c */
int				exec_simple_cmd(t_pip *cmd, t_exec *exec);

/* exec_cmd_utils.c */
void			execute_cmd(t_pip *cmd, t_exec *exec, char *cmd_path);
char			*get_expanded_command(t_pip *cmd, t_exec *exec);

/* exec_pipe.c */
int				exec_pipeline(t_pip *pipeline, t_exec *exec, int heredoc_fd);

/* exec_pipe_utils.c */
void			close_all_pipes(int pipes[][2], int count);
int				init_pids_array(t_exec *exec, pid_t **pids);
int				clean_fork_resources(pid_t *pids, int pipes[][2], int count);
int				cleanup_and_return(int pipes[][2], t_exec *exec, pid_t *pids);
int				fork_and_execute_pid(t_fork_params *params);

/* exec_pipe_exec.c */
void			execute_command(t_pip *cmd, t_exec *exec);
void			duplicate_redirect_fd(t_exec_params *params);
int				handle_builtin_in_child(t_exec_params *params,
					t_cmd_args *args);
pid_t			fork_and_execute_child(t_exec_params *params);
pid_t			fork_and_execute2(t_exec_params *params);

/* exec_pipe_helpers.c */
t_cmd_args		*get_command_args(t_pip *cmd);
char			*get_command_path(t_cmd_args *args, t_exec *exec);

/* exec_redir.c */
int				setup_redirections(t_parsed_cmd *cmd, t_process *process,
					t_exec *exec);
void			restore_redirections(t_process *process);

/* exec_redir_utils.c */
int				handle_output_redir(char *file, int append);
int				handle_input_redir(char *file);
int				create_output_files(t_parsed_cmd *cmd);
int				create_append_files(t_parsed_cmd *cmd);
int				backup_stdout(t_process *process);

/* exec_heredoc.c */
void			setup_parent_heredoc_signals(void);
int				handle_heredoc(t_parsed_cmd *cmd, t_exec *exec);

/* exec_heredoc_utils.c */
void			setup_child_heredoc_signals(void);
void			handle_heredoc_signal(int sig);
int				is_quoted_delimiter(char *delimiter);
char			*remove_quotes(char *delimiter);
char			*expand_heredoc_line(char *line, t_tools *tools);

/* exec_heredoc_processing.c */
int				wait_heredoc_child(pid_t pid, int pipe_fd[2], t_exec *exec);
int				setup_terminal_and_delimiter(struct termios *orig_term,
					char **clean_delim, char *delimiter, int *quoted);
int				process_heredoc_line(int fd, char *clean_delim, t_tools *tools,
					int quoted);
int				process_heredoc(int fd, char *delimiter, t_tools *tools);

/* exec_signals.c */
void			setup_parent_signals(void);
void			setup_child_signals(void);

/* exec_utils.c */
int				count_pipes(t_pip *pipeline);
void			close_pipe(int pipe_fd[2]);
void			wait_all_processes(t_exec *exec, int process_count);

/* exec_path.c */
char			*get_cmd_path(char *cmd, char **cmd_paths);

/* exec_builtins.c */
int				is_builtin(char *cmd);
int				handle_builtin(t_pip *cmd, t_exec *exec);
int				execute_builtin(t_cmd_args *args, t_exec *exec);

/* exec_error.c */
void			print_error(char *cmd, char *arg, char *message);
int				handle_cmd_error(char *cmd, char *error);
int				handle_file_error(char *cmd, char *file, char *error);
int				handle_pipe_error(void);
int				handle_fork_error(void);
int				handle_status(int status);

#endif