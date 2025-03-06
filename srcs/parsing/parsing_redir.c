/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/04 17:17:17 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	finalize_result(t_parsed_cmd *result, char *cmd_buffer)
{
	result->cmd = ft_strtrim(cmd_buffer, " \t");
	free(cmd_buffer);
	result->heredoc_delim[result->heredoc_count] = NULL;
	result->input_file[result->input_count] = NULL;
	result->output_file[result->output_count] = NULL;
	result->append_file[result->append_count] = NULL;
}

static t_parsed_cmd	*init_and_prepare(char *input)
{
	t_parsed_cmd	*result;

	if (!input || !check_redir_syntax(input))
		return (NULL);
	result = init_parsed_cmd();
	if (!result)
		return (NULL);
	result->full_cmd = ft_strdup(input);
	if (!result->full_cmd)
	{
		free_parsed_cmd(result);
		return (NULL);
	}
	return (result);
}

static void	parse_redir_loop(char *input, t_parsed_cmd *result, char *cmd_buf)
{
	char	*ptr;
	char	*cmd_ptr;
	int		in_quotes;
	char	quote_type;

	ptr = input;
	cmd_ptr = cmd_buf;
	in_quotes = 0;
	while (*ptr)
	{
		if (!in_quotes && (*ptr == '\'' || *ptr == '"'))
			process_quotes(&cmd_ptr, &ptr, &in_quotes, &quote_type);
		else if (in_quotes && *ptr == quote_type)
			process_quotes(&cmd_ptr, &ptr, &in_quotes, &quote_type);
		else if (!in_quotes)
			process_redirection(&ptr, result, &cmd_ptr);
		else
			*cmd_ptr++ = *ptr;
		ptr++;
	}
	*cmd_ptr = '\0';
}

t_parsed_cmd	*parse_redir(char *input)
{
	t_parsed_cmd	*result;
	char			*cmd_buffer;

	result = init_and_prepare(input);
	if (!result)
		return (NULL);
	cmd_buffer = malloc(ft_strlen(input) + 1);
	if (!cmd_buffer)
		return (free_parsed_cmd(result), NULL);
	parse_redir_loop(input, result, cmd_buffer);
	finalize_result(result, cmd_buffer);
	return (result);
}
