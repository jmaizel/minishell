/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:08:50 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/25 18:13:43 by jmaizel          ###   ########.fr       */
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
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

extern int					g_signal_received;

typedef struct s_tools
{
	char					**env;
	int						exit_code;
	struct s_simple_cmds	*cmds;
	struct s_tokens			*tokens;
}							t_tools;

typedef struct s_sep
{
	char					*cmd_sep;
	struct s_sep			*prev;
	struct s_sep			*next;
	struct s_pip			*pipcell;
}							t_sep;

typedef struct s_cmd_args
{
	char					**argv;
	int						argc;
	char					*cmd;
}							t_cmd_args;

typedef struct s_parsed_cmd
{
	char					*full_cmd;
	char					*cmd;
	char					**input_file;
	int						input_count;
	char					**output_file;
	int						output_count;
	char					**append_file;
	int						append_count;
	char					**heredoc_delim;
	int						heredoc_count;
}							t_parsed_cmd;

typedef struct s_pip
{
	char					*cmd_pipe;
	t_parsed_cmd			*redirection;
	struct s_pip			*next;
	struct s_pip			*prev;
	int						pip_count;
}							t_pip;

typedef struct s_expand
{
	size_t					i;
	size_t					j;
	int						in_quotes;
	size_t					size;
}							t_expand;

/*
** Environment functions (env.c)
*/
void						print_env_vars(t_tools *tools);
char						**get_env_paths(char **env, char *var_name);

/*
** Command argument functions (cmd_args_parse.c, cmd_args_utils.c)
*/
t_cmd_args					*parse_command_args(char *cmd_str);
void						print_cmd_args(t_cmd_args *cmd_args);
void						free_cmd_args(t_cmd_args *cmd_args);

/*
** Argument counting functions (count_args.c)
*/
int							count_args(char *str);

/*
** Redirections functions (redir_syntax.c, redir_init.c, redir_file.c,
	parse_redir.c)
*/
int							count_consecutive_chars(const char *input, int i);
int							check_redir_syntax(char *input);
t_parsed_cmd				*init_parsed_cmd(void);
char						*find_file_end(char *ptr);
void						handle_redirection(char **ptr, t_parsed_cmd *result,
								int type);

/*
** Parsed command functions (parse_display.c, parse_cleanup.c)
*/
void						print_parsed_command(t_parsed_cmd *cmd);
void						free_parsed_cmd(t_parsed_cmd *cmd);
void						parsing_line(char *user_input, t_tools *tools);
int							extract_arg(char **args, char *cmd_str, int i,
								int *j);
t_parsed_cmd				*parse_redir(char *input);
/*
** Cell functions (sep.c, parse_cleanup.c)
*/
t_sep						*create_cell(char *cmd_sep);
t_sep						*add_cell(t_sep *list, char *cmd_sep, int pos);
void						free_cell(t_sep *cell);
int							check_invalid_chars(const char *cmd);

/*
** Pipe functions (parsing_pipe.c, print_pipe_command.c)
*/
void						parse_pipes(t_sep *cell);
void						print_pipe_command(t_pip *pipe_cmd, int pipe_num);

/*
** Quote handling functions (quotes.c)
*/
char						*clean_quotes(char *str);

/*
** Signal functions (signals.c)
*/
void						setup_interactive_signals(void);
void						setup_exec_signals(void);
void						restore_signals(void);
char						*get_user_input(void);

/*
** Utility functions (utils.c)
*/
void						free_str_array(char **array);
int							ft_isspace(int c);

/*
** Prompt function (prompt.c)
*/
void						loop_prompt(t_tools *tools, char **env);

/*
** Expansion functions (expansion_core.c, expansion_utils.c,
	expansion_variables.c)
*/
char						*expand_str(const char *str, t_tools *tools);
char						*resize_buffer(char *result, size_t *size,
								size_t needed_size);
char						*get_var_value(const char *var_name, char **env);
char						*handle_variable(const char *str, size_t *i,
								t_tools *tools);
/*
** process
*/
void						process_redirection(char **ptr,
								t_parsed_cmd *result, char **cmd_ptr);
void	process_quotes(char **cmd_ptr, char **ptr, int *in_quotes,
		char *quote_type);
#endif