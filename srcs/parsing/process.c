/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:56:22 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 17:59:07 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_quotes(char **cmd_ptr, char **ptr, int *in_quotes,
		char *quote_type)
{
	if (!*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = **ptr;
	}
	else
		*in_quotes = 0;
	**cmd_ptr = **ptr;
	(*cmd_ptr)++;
}

void	process_redirection(char **ptr, t_parsed_cmd *result, char **cmd_ptr)
{
	if (ft_strncmp(*ptr, "<<", 2) == 0 || ft_strncmp(*ptr, ">>", 2) == 0)
		handle_redirection(ptr, result, 2);
	else if (**ptr == '<')
		handle_redirection(ptr, result, 0);
	else if (**ptr == '>')
		handle_redirection(ptr, result, 1);
	else
	{
		**cmd_ptr = **ptr;
		(*cmd_ptr)++;
	}
}
