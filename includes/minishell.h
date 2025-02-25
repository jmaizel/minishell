/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:08:50 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/25 13:43:41 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/ft_printf.h"
# include "../libft/includes/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

extern int	g_signal_received;

typedef struct s_tools
{
	char					**env;
	int						exit_code;
	struct s_simple_cmds	*cmds;
	struct s_tokens			*tokens;
}				t_tools;

typedef struct s_sep
{
	char			*cmd_sep;
	struct s_sep	*prev;
	struct s_sep	*next;
	struct s_pip	*pipcell;
}				t_sep;

typedef struct s_cmd_args
{
	char			**argv;
	int				argc;
	char			*cmd;
}				t_cmd_args;

typedef struct s_parsed_cmd
{
	char			*full_cmd;
	char			*cmd;
	char			**input_file;
	int				input_count;
	char			**output_file;
	int				output_count;
	char			**append_file;
	int				append_count;
	char			**heredoc_delim;
	int				heredoc_count;
}				t_parsed_cmd;

typedef struct s_pip
{
	char			*cmd_pipe;
	t_parsed_cmd	*redirection;
	struct s_pip	*next;
	struct s_pip	*prev;
	int				pip_count;
}				t_pip;

typedef struct s_expand
{
	size_t	i;
	size_t	j;
	int		in_quotes;
	size_t	size;
}	t_expand;

/*
** Environment functions
*/
void			print_env_vars(t_tools *tools);
char			**get_env_paths(char **env, char *var_name);

/*
** Parsing functions
*/
t_parsed_cmd	*parse_redir(char *input);
void			parsing_line(char *user_input, t_tools *tools);
void			free_parsed_cmd(t_parsed_cmd *cmd);
void			print_parsed_command(t_parsed_cmd *cmd);
void			parse_pipes(t_sep *cell);
void			loop_prompt(t_tools *tools, char **env);
t_sep			*add_cell(t_sep *list, char *cmd_sep, int pos);
t_sep			*create_cell(char *cmd_sep);
char			*get_user_input(void);
void			free_str_array(char **array);
int				ft_isspace(int c);
int				check_invalid_chars(const char *cmd);
char			*clean_quotes(char *str);
void			free_cell(t_sep *cell);
int				count_args(char *str);
void			print_pipe_command(t_pip *pipe_cmd, int pipe_num);

t_cmd_args		*parse_command_args(char *cmd_str);
void			print_cmd_args(t_cmd_args *cmd_args);
void			free_cmd_args(t_cmd_args *cmd_args);

/*
** Cleanup and signal functions
*/
void			cleanup_minishell(t_tools *tools);
void			setup_interactive_signals(void);
void			setup_exec_signals(void);
void			restore_signals(void);

/*
** Expansion functions
*/
char			*expand_str(const char *str, t_tools *tools);
char			*get_var_value(const char *var_name, char **env);
char			*expand_exit_status(int exit_code);


#endif