/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:27:41 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_file_end(char *ptr)
{
	int		in_quotes;
	char	quote_type;
	char	*file_end;

	in_quotes = 0;
	file_end = ptr;
	while (*file_end && (in_quotes || (!ft_isspace(*file_end)
				&& *file_end != '<' && *file_end != '>')))
	{
		if (!in_quotes && (*file_end == '"' || *file_end == '\''))
		{
			quote_type = *file_end;
			in_quotes = 1;
		}
		else if (in_quotes && *file_end == quote_type)
			in_quotes = 0;
		file_end++;
	}
	return (file_end);
}

void	handle_redirection(char **ptr, t_parsed_cmd *result, int type)
{
	char	*file_end;

	if (type == 2)
		*ptr += 2;
	else
		(*ptr)++;
	while (ft_isspace(**ptr))
		(*ptr)++;
	file_end = find_file_end(*ptr);
	if (type == 0)
		result->input_file[result->input_count++] = ft_substr(*ptr, 0, file_end
				- *ptr);
	else if (type == 1)
		result->output_file[result->output_count++] = ft_substr(*ptr, 0,
				file_end - *ptr);
	else if (type == 2)
	{
		if ((*ptr)[-2] == '<')
			result->heredoc_delim[result->heredoc_count++] = ft_substr(*ptr, 0,
					file_end - *ptr);
		else
			result->append_file[result->append_count++] = ft_substr(*ptr, 0,
					file_end - *ptr);
	}
	*ptr = file_end - 1;
}
