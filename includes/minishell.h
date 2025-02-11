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

typedef struct s_tools
{
	char			**env;
	// Variables d'environnement (tableau de chaînes de caractères)
	int				exit_code;
	// Code de sortie de la dernière commande exécutée
	struct s_simple_cmds *cmds; // Liste des commandes à exécuter
	struct s_tokens *tokens;    // Liste des tokens générés par le lexer
								// D'autres champs peuvent être ajoutés ici selon les besoins du projet
}					t_tools;

// CLEMENT : Structure principale qui contient une commande complète
typedef struct s_sep
{
	char *cmd_sep; // Commande complète brute
	struct s_sep	*prev;
	struct s_sep	*next;
	struct s_pip *pipcell; //  IMPORTANT: Point d'entrée de la liste des pipes
}					t_sep;

// CLEMENT : Structure pour les arguments de la commande (format execve)
typedef struct s_cmd_args
{
	char **argv; //  IMPORTANT: Tableau d'arguments pour execve
	int argc;    // Nombre d'arguments
	char *cmd;   // Nom de la commande
}					t_cmd_args;

// CLEMENT : Structure pour les redirections de chaque commande
typedef struct s_parsed_cmd
{
	char *full_cmd; // Commande complète avec redirections
	char *cmd;      //  Commande sans redirections

	//  IMPORTANT: Redirections d'entrée (<)
	char **input_file; // Tableau des fichiers d'entrée
	int input_count;   // Nombre de fichiers d'entrée

	//  IMPORTANT: Redirections de sortie (>)
	char **output_file; // Tableau des fichiers de sortie
	int output_count;   // Nombre de fichiers de sortie

	//  IMPORTANT: Redirections append (>>)
	char **append_file; // Tableau des fichiers append
	int append_count;   // Nombre de fichiers append

	//  IMPORTANT: Heredocs (<<)
	char **heredoc_delim; // Tableau des délimiteurs heredoc
	int heredoc_count;    // Nombre de heredocs
}					t_parsed_cmd;

// CLEMENT :  STRUCTURE PRINCIPALE POUR L'EXÉCUTION
// C'est ici que tu trouveras toutes les infos pour chaque commande du pipeline
typedef struct s_pip
{
	char *cmd_pipe;            // Commande brute pour ce pipe
	t_parsed_cmd *redirection; //  Toutes les redirections pour cette commande
	struct s_pip *next;        //  Commande suivante dans le pipeline
	struct s_pip *prev;        // Commande précédente
	int pip_count;             // Nombre total de pipes
}					t_pip;

// CLEMENT : Pour accéder aux informations :
// 1. Tu reçois un t_sep *cell
// 2. Tu accèdes à la liste des pipes avec cell->pipcell
// 3. Pour chaque pipe (t_pip):
//    - Tu as la commande dans cmd_pipe
//    - Tu as toutes les redirections dans redirection
//    - Tu passes au pipe suivant avec next
//
// Exemple de parcours :
// t_pip *current = cell->pipcell;
// while (current)
// {
//     // Traiter les redirections avec current->redirection
//     // Exécuter la commande
//     current = current->next;
// }

// env :
void				print_env_vars(t_tools *tools);
char				**get_env_paths(char **env, char *var_name);

// parsing :
t_parsed_cmd		*parse_redir(char *input);
void				parsing_line(char *user_input, t_tools *tools);
void				free_parsed_cmd(t_parsed_cmd *cmd);
void				print_parsed_command(t_parsed_cmd *cmd);
void				parse_pipes(t_sep *cell);
void				loop_prompt(t_tools *tools, char **env);
t_sep				*add_cell(t_sep *list, char *cmd_sep, int pos);
t_sep				*create_cell(char *cmd_sep);
void				setup_signals(void);
void				handle_signal(int sig);
char				*get_user_input(void);
void				free_str_array(char **array);
int					ft_isspace(int c);
int					check_invalid_chars(const char *cmd);
char				*clean_quotes(char *str);
void				free_cell(t_sep *cell);
int					count_args(char *str);
void				print_pipe_command(t_pip *pipe_cmd, int pipe_num);

t_cmd_args			*parse_command_args(char *cmd_str);
void				print_cmd_args(t_cmd_args *cmd_args);
void				free_cmd_args(t_cmd_args *cmd_args);
void				cleanup_minishell(t_tools *tools);

#endif