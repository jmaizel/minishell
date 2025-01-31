/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:05:25 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 13:54:13 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

int	builtin_pwd(t_parsed_cmd *cmd, t_tools *tools)
{
   char	*pwd;
   char	**args;

   (void)tools;
   if (!cmd)
   	return (ERR_INVALID_CMD);

   args = ft_split(cmd->full_cmd, ' ');
   if (!args)
   	return (ERR_MALLOC_FAILURE);

   if (args[1])
   {
   	ft_putendl_fd("pwd: too many arguments", STDERR_FILENO);
   	free_str_array(args);
   	return (ERR_INVALID_CMD);
   }
   free_str_array(args);

   pwd = getcwd(NULL, 0);
   if (!pwd)
   {
   	perror("pwd");
   	return (ERR_GETCWD_FAILED);
   }
   ft_putendl_fd(pwd, STDOUT_FILENO);
   free(pwd);
   return (SUCCESS);
}