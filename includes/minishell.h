#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
#include <stdbool.h>
#include "../libft/includes/libft.h"
#include "../libft/includes/ft_printf.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef	struct			s_sep
{
	char				*cmd_sep;
	struct s_sep		*prev;
	struct s_sep		*next;
	struct s_pip		*pipcell;
}						t_sep;

struct s_simple_cmds;

typedef enum e_token_type
{
    TOK_COMMAND,      // Représente une commande simple
    TOK_ARGUMENT,     // Représente un argument
    TOK_PIPE,         // Représente un pipe '|'
    TOK_HEREDOC,
    TOK_INPUT,
    TOK_OUTPUT,
    TOK_APPEND,
    TOK_REDIRECTION,  // Représente une redirection, comme '>' ou '<'
    TOK_PIPE_OUT,     // Représente un pipe sortant
    TOK_PIPE_IN,      // Représente un pipe entrant
    TOK_EOF,          // Fin de fichier
    TOK_INVALID       // Token invalide
} t_token_type;


typedef struct s_tools
{
    char **env;            // Variables d'environnement (tableau de chaînes de caractères)
    int exit_code;         // Code de sortie de la dernière commande exécutée
    struct s_simple_cmds *cmds;   // Liste des commandes à exécuter
    struct s_tokens *tokens;      // Liste des tokens générés par le lexer
    // D'autres champs peuvent être ajoutés ici selon les besoins du projet
} t_tools;


typedef struct s_tokens
{
    t_token_type type;       // Le type du token (par exemple, commande, argument, pipe, etc.)
    char *value;             // La valeur littérale du token (par exemple, le texte de la commande ou de l'argument)
    struct s_tokens *next;   // Pointeur vers le token suivant
} t_tokens;


typedef struct s_lexer
{
    char *str;               // Chaîne à analyser
    t_tokens token;          // Le token analysé
    int i;                   // Position actuelle dans la chaîne
    struct s_lexer *next;    // Pointeur vers le lexer suivant
    struct s_lexer *prev;    // Pointeur vers le lexer précédent
} t_lexer;

typedef struct s_cmd_args {
    char **argv;           // Tableau d'arguments
    int argc;              // Nombre d'arguments
    char *cmd;             // Première partie (commande principale)
} t_cmd_args;

typedef struct s_parsed_cmd {
    char *full_cmd;          // Commande complète
    char *cmd;               // Commande nettoyée
    char **input_file;      // Tableau de fichiers d'entrée
    int input_count;         // Nombre de fichiers d'entrée
    char **output_file;     // Tableau de fichiers de sortie
    int output_count;        // Nombre de fichiers de sortie
    char **append_file;       // Fichier pour >>
    int append_count; //nombre de append
    char **heredoc_delim;   // Tableau de délimiteurs pour 
    int heredoc_count;       // Nombre de délimiteurs
} t_parsed_cmd;

typedef struct s_pip 
{
    char *cmd_pipe;         // Commande dans le pipe
    t_parsed_cmd *redirection;  // Redirection pour cette commande
    struct s_pip *next;
    struct s_pip *prev;
    int pip_count; //nombre de pipe
} t_pip;


//env :
void	print_env_vars(t_tools *tools);
char	**get_env_paths(char **env, char *var_name);

//parsing :
t_parsed_cmd *parse_redir(char *input);
void	parsing_line(char *user_input, t_tools *tools);
void	free_parsed_cmd(t_parsed_cmd *cmd);
void	print_parsed_command(t_parsed_cmd *cmd);
void	parse_pipes(t_sep *cell);
void	loop_prompt(t_tools *tools, char **env);
t_sep	*add_cell(t_sep *list, char *cmd_sep, int pos);
t_sep	*create_cell(char *cmd_sep);
void	setup_signals(void);
void	handle_signal(int sig);
char	*get_user_input(void);
void	free_str_array(char **array);
int	ft_isspace(int c);

t_cmd_args	*parse_command_args(char *cmd_str);
void	print_cmd_args(t_cmd_args *cmd_args);
void	free_cmd_args(t_cmd_args *cmd_args);

#endif