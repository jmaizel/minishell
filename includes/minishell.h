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
// Déclaration préalable (forward declaration)
struct s_simple_cmds;

typedef enum e_token_type
{
    TOK_COMMAND,      // Représente une commande simple
    TOK_ARGUMENT,     // Représente un argument
    TOK_PIPE,         // Représente un pipe '|'
    TOK_REDIRECTION,  // Représente une redirection, comme '>' ou '<'
    TOK_PIPE_OUT,     // Représente un pipe sortant
    TOK_PIPE_IN,      // Représente un pipe entrant
    TOK_EOF,          // Fin de fichier
    TOK_INVALID       // Token invalide
} t_token_type;

// Structure principale contenant les données et l'état global de l'application
typedef struct s_tools
{
    char **env;            // Variables d'environnement (tableau de chaînes de caractères)
    int exit_code;         // Code de sortie de la dernière commande exécutée
    struct s_simple_cmds *cmds;   // Liste des commandes à exécuter
    struct s_tokens *tokens;      // Liste des tokens générés par le lexer
    // D'autres champs peuvent être ajoutés ici selon les besoins du projet
} t_tools;

// Structure représentant un token
typedef struct s_tokens
{
    t_token_type type;       // Le type du token (par exemple, commande, argument, pipe, etc.)
    char *value;             // La valeur littérale du token (par exemple, le texte de la commande ou de l'argument)
    struct s_tokens *next;   // Pointeur vers le token suivant
} t_tokens;

// Structure représentant un lexer
typedef struct s_lexer
{
    char *str;               // Chaîne à analyser
    t_tokens token;          // Le token analysé
    int i;                   // Position actuelle dans la chaîne
    struct s_lexer *next;    // Pointeur vers le lexer suivant
    struct s_lexer *prev;    // Pointeur vers le lexer précédent
} t_lexer;

// Structure représentant une commande simple
typedef struct s_simple_cmds
{
    char **str;                          // Tableau de chaînes (arguments ou commandes)
    int (*builtin)(t_tools *, struct s_simple_cmds *); // Fonction builtin (si applicable)
    int num_redirections;                // Nombre de redirections
    char *hd_file_name;                  // Nom du fichier de redirection
    t_lexer *redirections;               // Liste des redirections associées
    struct s_simple_cmds *next;          // Pointeur vers la commande suivante
    struct s_simple_cmds *prev;          // Pointeur vers la commande précédente
} t_simple_cmds;

// Ajout d'une structure de redirection
typedef struct s_redirection {
    char *input_file;
    char *output_file;
    char *append_file;
    char *heredoc;
    int type;
} t_redirection;

// Modification de la structure s_pip pour inclure les redirections
typedef struct s_pip 
{
    char *cmd_pipe;         // Commande dans le pipe
    t_redirection *redirection;  // Redirection pour cette commande
    struct s_pip *next;
    struct s_pip *prev;
} t_pip;


//builtins

//env
char	**get_env_paths(char **env,char *var_name);
void	print_env_vars(t_tools *tools);

//propmpt
void	loop_prompt(t_tools *tools, char **env);
void	parsing_line(char *user_input, t_tools *tools);
t_sep	*create_cell(char *cmd_sep);
t_sep	*add_cell(t_sep *list, char *cmd_sep, int pos);
void	print_list(t_sep *list);
char	*get_user_input(void);
void	handle_signal(int sig);
void	setup_signals(void);

//exec

//tools

//parsing
t_redirection	*parse_redirections(char *cmd);
void	parse_pipes(t_sep *cell);


//utils
void	free_str_array(char **array);
int	ft_isspace(int c);

#endif