#ifndef EXPANSION_H
# define EXPANSION_H

# include "../includes/execution.h" 
/*
   Ou "../includes/minishell.h" 
   si c'est là que se trouve t_env_manager, ft_strdup, etc.
*/

/**
 * @brief Expanse les variables d'environnement (ex: "$PWD", "$HOME", "$?") 
 *        dans la chaîne 'str'. Gère également les guillemets simples/doubles 
 *        pour l'instant.
 * 
 * @param str      Chaîne potentiellement contenant des variables.
 * @param env_mgr  Pointeur vers le gestionnaire d'environnement 
 *                 (contient un t_tools et son exit_code, etc.).
 * @return char*   Nouvelle chaîne (malloc) avec les variables substituées,
 *                 ou NULL si erreur.
 */
char	*expand_variables(const char *str, t_env_manager *env_mgr);

#endif /* EXPANSION_H */