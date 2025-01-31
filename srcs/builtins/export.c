/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:38:16 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/31 13:53:32 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtins.h"

static int	is_valid_var_name(const char *var)
{
   int	i;

   if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
   	return (0);
   i = 1;
   while (var[i] && var[i] != '=')
   {
   	if (!ft_isalnum(var[i]) && var[i] != '_')
   		return (0);
   	i++;
   }
   return (1);
}

static void	print_sorted_env(char **env)
{
   char	**sorted_env;
   int		i;

   sorted_env = duplicate_env(env);
   if (!sorted_env)
   	return ;
   sort_string_array(sorted_env);
   i = 0;
   while (sorted_env[i])
   {
   	ft_putstr_fd("declare -x ", STDOUT_FILENO);
   	ft_putendl_fd(sorted_env[i], STDOUT_FILENO);
   	i++;
   }
   free_env(sorted_env);
}

static int	add_or_update_var(const char *arg, t_tools *tools)
{
   if (ft_strchr(arg, '='))
   	return (add_env_var((char *)arg, &tools->env));
   if (get_env_var(arg, tools->env))
   	return (SUCCESS);
   return (add_env_var((char *)arg, &tools->env));
}

int	builtin_export(t_parsed_cmd *cmd, t_tools *tools)
{
   char	**args;
   int		i;
   int		ret;

   if (!cmd || !tools || !tools->env)
   	return (ERR_INVALID_CMD);

   args = ft_split(cmd->full_cmd, ' ');
   if (!args)
   	return (ERR_MALLOC_FAILURE);

   if (!args[1])
   {
   	print_sorted_env(tools->env);
   	free_str_array(args);
   	return (SUCCESS);
   }

   i = 1;
   ret = SUCCESS;
   while (args[i])
   {
   	if (is_valid_var_name(args[i]))
   	{
   		if (add_or_update_var(args[i], tools) != SUCCESS)
   			ret = ERR_MALLOC_FAILURE;
   	}
   	else
   	{
   		ft_putstr_fd("export: `", STDERR_FILENO);
   		ft_putstr_fd(args[i], STDERR_FILENO);
   		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
   		ret = ERR_INVALID_CMD;
   	}
   	i++;
   }
   free_str_array(args);
   return (ret);
}
