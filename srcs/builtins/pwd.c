/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:05:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/28 14:40:48 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	builtin_pwd(t_simple_cmds *cmd, t_tools *tools)
{
	char	*cwd;

	(void)cmd;
	(void)tools;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd: getcwd failed");
		return (ERR_GETCWD_FAILED);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (SUCCESS);
}
