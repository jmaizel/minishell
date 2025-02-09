/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:22 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/09 18:33:01 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

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
int		exec_pipeline(t_pip *pipeline, t_exec *exec);

/* exec_redir.c */
int		setup_redirections(t_parsed_cmd *cmd, t_process *process);
void	restore_redirections(t_process *process);

/* exec_heredoc.c */
int		handle_heredoc(char *delimiter);

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

#endif