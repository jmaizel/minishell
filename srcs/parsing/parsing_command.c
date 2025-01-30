/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:23:51 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/30 14:09:52 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_parsed_cmd	*parse_command(char *input)
{
	t_parsed_cmd *result = malloc(sizeof(t_parsed_cmd));
	ft_memset(result, 0, sizeof(t_parsed_cmd));

	result->heredoc_delim = malloc(sizeof(char*) *10);
	result->heredoc_count = 0;

	char *ptr = input;
	int in_quotes = 0;
	char quote_type = 0;
	char *cmd_buffer = malloc(ft_strlen(input) + 1);
	char *cmd_ptr = cmd_buffer;

	// Initialiser la commande complète
	result->full_cmd = ft_strdup(input);
	while (*ptr)
	{
		// Gestion des quotes
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
		// Si pas dans les quotes, chercher les redirections
		if (!in_quotes)
		{
			// Gestion des redirections
			 if (strncmp(ptr, "<<", 2) == 0)
            {
                ptr += 2;
                while (ft_isspace(*ptr)) ptr++;
                
                char *delim_end = ptr;
                while (*delim_end && !ft_isspace(*delim_end)) 
                    delim_end++;
                
                // Stocker le délimiteur dans le tableau
                result->heredoc_delim[result->heredoc_count] = 
                    ft_substr(ptr, 0, delim_end - ptr);
                result->heredoc_count++;

                ptr = delim_end;
            }
			else if (strncmp(ptr, ">>", 2) == 0)
			{
				ptr += 2;
				while (ft_isspace(*ptr))
					ptr++;
				char *file_end = ptr;
				while (*file_end && !ft_isspace(*file_end))
					file_end++;
				result->append_file = ft_substr(ptr, 0, file_end - ptr);
				ptr = file_end;
			}
			else if (*ptr == '<')
			{
				ptr++;
				while (ft_isspace(*ptr))
					ptr++;
				char *file_end = ptr;
				while (*file_end && !ft_isspace(*file_end))
					file_end++;
				result->input_file = ft_substr(ptr, 0, file_end - ptr);
				ptr = file_end;
			}
			else if (*ptr == '>')
			{
				ptr++;
				while (ft_isspace(*ptr))
					ptr++;
				char *file_end = ptr;
				while (*file_end && !ft_isspace(*file_end))
					file_end++;
				result->output_file = ft_substr(ptr, 0, file_end - ptr);
				ptr = file_end;
			}
			else
			{
				// Copier les caractères normaux
				*cmd_ptr++ = *ptr;
			}
		}
		else
		{
			// Dans les quotes, copier tous les caractères
			*cmd_ptr++ = *ptr;
		}
		ptr++;
	}
	// Terminer la chaîne
	*cmd_ptr = '\0';
	// Libérer les espaces en tête et queue
	result->cmd = ft_strtrim(cmd_buffer, " \t");
	free(cmd_buffer);
	result->heredoc_delim[result->heredoc_count] = NULL;
	return (result);
}
