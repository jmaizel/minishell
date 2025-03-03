/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:46 by jacobmaizel       #+#    #+#             */
/*   Updated: 2025/03/03 14:46:54 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"
#include "../includes/minishell.h"

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
