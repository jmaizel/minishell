/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:46 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/02/12 14:09:13 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

// fonction pour gerer pwd :
// 1. PATH_MAX est une constante systeme qui definit la longueur maximale
// d un chemin (generalement 4096)
// 2. getcwd est une fonction systeme qui stock le chemin et renvoie NULL en
// cas d erreur
// 3.strerror(errno) converti le code d erreur en message
int	builtin_pwd(t_tools *tools, char **args)
{
	char	current_pwd[PATH_MAX];

	(void)args;
	(void)tools;
	if (!getcwd(current_pwd, PATH_MAX))
	{
		ft_printf("pwd: %s\n", strerror(errno));
		return (1);
	}
	ft_printf("%s\n", current_pwd);
	return (0);
}
