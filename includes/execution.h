/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:22 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:37:14 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <termios.h>

# define PIPE_READ 0
# define PIPE_WRITE 1

typedef struct s_process
{
	pid_t	pid;
	int		pipe_fd[2];
	int		stdin_backup;
	int		stdout_backup;
}	t_process;

typedef struct s_exec
{
	t_tools		*tools;
	t_process	process;
	int			exit_status;
	int			pipe_count;
	char		**cmd_paths;
}	t_exec;

/* exec.c */
void	init_exec_struct(t_exec *exec, t_tools *tools);
int		exec_commands(t_sep *cell, t_tools *tools);

/* exec_cmd.c */
int		exec_simple_cmd(t_pip *cmd, t_exec *exec);

/* exec_pipe.c */
int		exec_pipeline(t_pip *pipeline, t_exec *exec, int heredoc_fd);

/* exec_redir.c */
int		setup_redirections(t_parsed_cmd *cmd, t_process *process, t_exec *exec);
void	restore_redirections(t_process *process);

/* exec_heredoc.c & exec_heredoc_2.c */
void	setup_child_heredoc_signals(void);
void	setup_parent_heredoc_signals(void);
int		handle_heredoc(t_parsed_cmd *cmd, t_exec *exec);
int		is_quoted_delimiter(char *delimiter);
char	*remove_quotes(char *delimiter);
char	*expand_heredoc_line(char *line, t_tools *tools);

/* exec_signals.c */
void	setup_parent_signals(void);
void	setup_child_signals(void);

/* exec_utils.c */
int		count_pipes(t_pip *pipeline);
void	close_pipe(int pipe_fd[2]);
void	wait_all_processes(t_exec *exec, int process_count);

/* exec_path.c */
char	*get_cmd_path(char *cmd, char **cmd_paths);

/* exec_builtins.c */
int		is_builtin(char *cmd);
int		handle_builtin(t_pip *cmd, t_exec *exec);
int		execute_builtin(t_cmd_args *args, t_exec *exec);

/* exec_error.c */
void	print_error(char *cmd, char *arg, char *message);
int		handle_cmd_error(char *cmd, char *error);
int		handle_file_error(char *cmd, char *file, char *error);
int		handle_pipe_error(void);
int		handle_fork_error(void);

#endif