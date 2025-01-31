/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 11:33:29 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_parsed_cmd *parse_redir(char *input)
{
    t_parsed_cmd *result = malloc(sizeof(t_parsed_cmd));
    ft_memset(result, 0, sizeof(t_parsed_cmd));
    

    result->heredoc_delim = malloc(sizeof(char*) * 10);
    result->input_file = malloc(sizeof(char*) * 10);
    result->output_file = malloc(sizeof(char*) * 10);
	 result->append_file = malloc(sizeof(char*) * 10);
    result->heredoc_count = 0;
    result->input_count = 0;
    result->output_count = 0;
	result->append_count = 0;

    char *ptr = input;
    int in_quotes = 0;
    char quote_type = 0;
    char *cmd_buffer = malloc(ft_strlen(input) + 1);
    char *cmd_ptr = cmd_buffer;

    result->full_cmd = ft_strdup(input);

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
            quote_type = 0;
            *cmd_ptr++ = *ptr;
        }

        if (!in_quotes)
        {
            if (strncmp(ptr, "<<", 2) == 0)
            {
                ptr += 2;
                while (ft_isspace(*ptr)) ptr++;
                char *delim_end = ptr;
                while (*delim_end && !ft_isspace(*delim_end))
                    delim_end++;
                
                result->heredoc_delim[result->heredoc_count] = 
                    ft_substr(ptr, 0, delim_end - ptr);
                result->heredoc_count++;
                ptr = delim_end;
            }
			else if (strncmp(ptr, ">>", 2) == 0)
        {
            ptr += 2;
            while (ft_isspace(*ptr)) ptr++;
            char *file_end = ptr;
            while (*file_end && !ft_isspace(*file_end))
                file_end++;
            result->append_file[result->append_count] =
                ft_substr(ptr, 0, file_end - ptr);
            result->append_count++;
            ptr = file_end;
        }
            else if (*ptr == '<')
            {
                ptr++;
                while (ft_isspace(*ptr)) ptr++;
                char *file_end = ptr;
                while (*file_end && !ft_isspace(*file_end))
                    file_end++;
                
                result->input_file[result->input_count] = 
                    ft_substr(ptr, 0, file_end - ptr);
                result->input_count++;
                ptr = file_end;
            }
            else if (*ptr == '>')
            {
                ptr++;
                while (ft_isspace(*ptr)) ptr++;
                char *file_end = ptr;
                while (*file_end && !ft_isspace(*file_end))
                    file_end++;
                
                result->output_file[result->output_count] = 
                    ft_substr(ptr, 0, file_end - ptr);
                result->output_count++;
                ptr = file_end;
            }
            else
            {
                *cmd_ptr++ = *ptr;
            }
        }
        else
        {
            *cmd_ptr++ = *ptr;
        }
        
        ptr++;
    }

    *cmd_ptr = '\0';
    result->cmd = ft_strtrim(cmd_buffer, " \t");
    free(cmd_buffer);

    result->heredoc_delim[result->heredoc_count] = NULL;
    result->input_file[result->input_count] = NULL;
    result->output_file[result->output_count] = NULL;
	result->append_file[result->append_count] = NULL;

    return result;
}

