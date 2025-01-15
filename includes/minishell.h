#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
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

typedef struct s_tokens
{
    t_token_type type;   // Le type du token (par exemple, commande, argument, pipe, etc.)
    char *value;         // La valeur littérale du token (par exemple, le texte de la commande ou de l'argument)
    struct s_tokens *next;  // Pointeur vers le token suivant
} t_tokens;

typedef struct s_tools
{
    char **env;            // Variables d'environnement (tableau de chaînes de caractères)
    int exit_code;         // Code de sortie de la dernière commande exécutée
    t_simple_cmds *cmds;   // Liste des commandes à exécuter
    t_tokens *tokens;      // Liste des tokens générés par le lexer
    // D'autres champs peuvent être ajoutés ici selon les besoins du projet
} t_tools;

typedef struct s_lexer
{
	char    	*str;
    t_tokens        token;
	int		i;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_simple_cmds
{
	char                    **str;
	int                     (*builtin)(t_tools *, struct s_simple_cmds *);
	int                     num_redirections;
	char                    *hd_file_name;
	t_lexer                 *redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

//builtins

//env

//exec

//tools

//parsing
int	is_quote_closed(char *str);

#endif