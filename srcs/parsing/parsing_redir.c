/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/13 21:58:20 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	print_syntax_error(char c)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	return (0);
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
				return (print_syntax_error(input[i]));
			if (input[i] == '<' && input[i + count] == '>')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `>'\n",
					2);
				return (0);
			}
			i += count - 1;
		}
		i++;
	}
	return (1);
}

static t_parsed_cmd	*init_parsed_cmd(char *input)
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
	result->full_cmd = ft_strdup(input);
	if (!result->full_cmd)
	{
		free_parsed_cmd(result);
		return (NULL);
	}
	return (result);
}

static void	handle_heredoc(char **ptr, t_parsed_cmd *result)
{
	char	*file_end;

	*ptr += 2;
	while (ft_isspace(**ptr))
		(*ptr)++;
	file_end = find_file_end(*ptr);
	if (result->heredoc_count < MAX_REDIR - 1)
		result->heredoc_delim[result->heredoc_count++]
			= ft_substr(*ptr, 0, file_end - *ptr);
	*ptr = file_end - 1;
}

static void	handle_append(char **ptr, t_parsed_cmd *result)
{
	char	*file_end;

	*ptr += 2;
	while (ft_isspace(**ptr))
		(*ptr)++;
	file_end = find_file_end(*ptr);
	if (result->append_count < MAX_REDIR - 1)
		result->append_file[result->append_count++]
			= ft_substr(*ptr, 0, file_end - *ptr);
	*ptr = file_end - 1;
}

static void	handle_input(char **ptr, t_parsed_cmd *result)
{
	char	*file_end;

	(*ptr)++;
	while (ft_isspace(**ptr))
		(*ptr)++;
	file_end = find_file_end(*ptr);
	if (result->input_count < MAX_REDIR - 1)
		result->input_file[result->input_count++]
			= ft_substr(*ptr, 0, file_end - *ptr);
	*ptr = file_end - 1;
}

static void	handle_output(char **ptr, t_parsed_cmd *result)
{
	char	*file_end;

	(*ptr)++;
	while (ft_isspace(**ptr))
		(*ptr)++;
	file_end = find_file_end(*ptr);
	if (result->output_count < MAX_REDIR - 1)
		result->output_file[result->output_count++]
			= ft_substr(*ptr, 0, file_end - *ptr);
	*ptr = file_end - 1;
}

static void	handle_quotes(char **ptr, char **cmd_ptr, int *in_quotes,
		char *quote_type)
{
	if (!*in_quotes && (**ptr == '\'' || **ptr == '"'))
	{
		*in_quotes = 1;
		*quote_type = **ptr;
		*(*cmd_ptr)++ = **ptr;
	}
	else if (*in_quotes && **ptr == *quote_type)
	{
		*in_quotes = 0;
		*(*cmd_ptr)++ = **ptr;
	}
}

static void	process_redirections(char **ptr, t_parsed_cmd *result,
		char **cmd_ptr)
{
	if (ft_strncmp(*ptr, "<<", 2) == 0)
		handle_heredoc(ptr, result);
	else if (ft_strncmp(*ptr, ">>", 2) == 0)
		handle_append(ptr, result);
	else if (**ptr == '<')
		handle_input(ptr, result);
	else if (**ptr == '>')
		handle_output(ptr, result);
	else
		*(*cmd_ptr)++ = **ptr;
}

static void	process_input(char *input, t_parsed_cmd *result, char *cmd_buffer)
{
	char	*ptr;
	char	*cmd_ptr;
	int		in_quotes;
	char	quote_type;

	ptr = input;
	cmd_ptr = cmd_buffer;
	in_quotes = 0;
	while (*ptr)
	{
		if (!in_quotes && (*ptr == '\'' || *ptr == '"'))
			handle_quotes(&ptr, &cmd_ptr, &in_quotes, &quote_type);
		else if (in_quotes)
			*cmd_ptr++ = *ptr;
		else
			process_redirections(&ptr, result, &cmd_ptr);
		ptr++;
	}
	*cmd_ptr = '\0';
}

t_parsed_cmd	*parse_redir(char *input)
{
	t_parsed_cmd	*result;
	char			*cmd_buffer;

	if (!input)
		return (NULL);
	if (!check_redir_syntax(input))
		return (NULL);
	result = init_parsed_cmd(input);
	if (!result)
		return (NULL);
	cmd_buffer = malloc(ft_strlen(input) + 1);
	if (!cmd_buffer)
	{
		free_parsed_cmd(result);
		return (NULL);
	}
	process_input(input, result, cmd_buffer);
	result->cmd = ft_strtrim(cmd_buffer, " \t");
	free(cmd_buffer);
	result->heredoc_delim[result->heredoc_count] = NULL;
	result->input_file[result->input_count] = NULL;
	result->output_file[result->output_count] = NULL;
	result->append_file[result->append_count] = NULL;
	return (result);
}
