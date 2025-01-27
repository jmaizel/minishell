/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:43:51 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 11:54:48 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = malloc(1024);
	if (!cwd)
		return (1);
	
	if (getcwd(cwd, 1024) != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		free(cwd);
		return (0);
	}
	
	perror("pwd");
	free(cwd);
	return (1);
}