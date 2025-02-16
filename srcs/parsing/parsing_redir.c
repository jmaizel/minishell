/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/16 20:32:28 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define MAX_REDIR 10

static int	count_consecutive_chars(const char *input, int i)
{
	char	c;
	int		count;

	c = input[i];
	count = 1;
	while (input[i + 1] == c)
	{
		count++;
		i++;
	}
	return (count);
}

static int	check_redir_syntax(char *input)
{
	int	i;
	int	count;

	i = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			count = count_consecutive_chars(input, i);
			if (count > 2)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				ft_putchar_fd(input[i], 2);
				ft_putstr_fd("'\n", 2);
				return (0);
			}
			i += count;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if (!input[i] || input[i] == '<' || input[i] == '>')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (0);
			}
			i--;
		}
		i++;
	}
	return (1);
}

static t_parsed_cmd	*init_parsed_cmd(void)
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
	if (!result->heredoc_delim || !result->input_file
		|| !result->output_file || !result->append_file)
	{
		free_parsed_cmd(result);
		return (NULL);
	}
	return (result);
}

static char	*find_file_end(char *ptr)
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

static void	handle_redirection(char **ptr, t_parsed_cmd *result, int type)
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
		result->input_file[result->input_count++]
			= ft_substr(*ptr, 0, file_end - *ptr);
	else if (type == 1)
		result->output_file[result->output_count++]
			= ft_substr(*ptr, 0, file_end - *ptr);
	else if (type == 2)
	{
		if ((*ptr)[-2] == '<')
			result->heredoc_delim[result->heredoc_count++]
				= ft_substr(*ptr, 0, file_end - *ptr);
		else
			result->append_file[result->append_count++]
				= ft_substr(*ptr, 0, file_end - *ptr);
	}
	*ptr = file_end - 1;
}


t_parsed_cmd	*parse_redir(char *input)
{
	t_parsed_cmd	*result;
	char			*ptr;
	char			*cmd_buffer;
	char			*cmd_ptr;
	int				in_quotes;
	char			quote_type;

	if (!input || !check_redir_syntax(input))
		return (NULL);
	result = init_parsed_cmd();
	if (!result || !(cmd_buffer = malloc(ft_strlen(input) + 1)))
		return (NULL);
	result->full_cmd = ft_strdup(input);
	ptr = input;
	cmd_ptr = cmd_buffer;
	in_quotes = 0;
	while (*ptr)
	{
		if (!in_quotes && (*ptr == '\'' || *ptr == '"'))
		{
			in_quotes = 1;
			quote_type = *ptr;
			*cmd_ptr++ = *ptr;
		}
		else if (in_quotes && *ptr == quote_type)
		{
			in_quotes = 0;
			*cmd_ptr++ = *ptr;
		}
		else if (!in_quotes)
		{
			if (ft_strncmp(ptr, "<<", 2) == 0 || ft_strncmp(ptr, ">>", 2) == 0)
				handle_redirection(&ptr, result, 2);
			else if (*ptr == '<')
				handle_redirection(&ptr, result, 0);
			else if (*ptr == '>')
				handle_redirection(&ptr, result, 1);
			else
				*cmd_ptr++ = *ptr;
		}
		else
			*cmd_ptr++ = *ptr;
		ptr++;
	}
	*cmd_ptr = '\0';
	result->cmd = ft_strtrim(cmd_buffer, " \t");
	free(cmd_buffer);
	result->heredoc_delim[result->heredoc_count] = NULL;
	result->input_file[result->input_count] = NULL;
	result->output_file[result->output_count] = NULL;
	result->append_file[result->append_count] = NULL;
	return (result);
}
