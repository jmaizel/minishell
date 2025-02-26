/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:27:31 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define MAX_REDIR 10

t_parsed_cmd	*init_parsed_cmd(void)
{
	t_parsed_cmd	*result;

	result = malloc(sizeof(t_parsed_cmd));
	if (!result)
		return (NULL);
	ft_memset(result, 0, sizeof(t_parsed_cmd));
	result->heredoc_delim = malloc(sizeof(char *) * MAX_REDIR);
	result->input_file = malloc(sizeof(char *) * MAX_REDIR);
	result->output_file = malloc(sizeof(char *) * MAX_REDIR);
	result->append_file = malloc(sizeof(char *) * MAX_REDIR);
	if (!result->heredoc_delim || !result->input_file || !result->output_file
		|| !result->append_file)
	{
		free_parsed_cmd(result);
		return (NULL);
	}
	return (result);
}
