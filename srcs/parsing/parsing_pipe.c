/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jacobmaizel <jacobmaizel@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:04:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/29 14:49:43 by jacobmaizel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction pour extraire le fichier de redirection
char	*extract_redirection_file(char *redir_ptr)
{
	char	*file_start;
	char	*file_end;
	char	*file;

	while (*redir_ptr && (*redir_ptr == '<' || *redir_ptr == '>'))
		redir_ptr++;
	while (*redir_ptr && ft_isspace(*redir_ptr))
		redir_ptr++;
	file_start = redir_ptr;
	file_end = file_start;
	while (*file_end && !ft_isspace(*file_end))
		file_end++;
	file = ft_substr(file_start, 0, file_end - file_start);
	return (file);
}

// fonction pour parser les redirections
t_redirection	*parse_redirections(char *cmd)
{
	t_redirection	*redir;
	char			*ptr;
	int				in_quotes;
	char			quote_char;
	char			*delim_end;
	char			*file_end;

	redir = malloc(sizeof(t_redirection));
	redir->input_file = NULL;
	redir->output_file = NULL;
	redir->append_file = NULL;
	redir->heredoc = NULL;
	ptr = cmd;
	in_quotes = 0;
	quote_char = 0;
	while (*ptr)
	{
		// Gestion des quotes
		if (!in_quotes && (*ptr == '\'' || *ptr == '"'))
		{
			in_quotes = 1;
			quote_char = *ptr;
		}
		else if (in_quotes && *ptr == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
		}
		// Si pas dans les quotes, analyser les redirections
		if (!in_quotes)
		{
			if (strncmp(ptr, "<<", 2) == 0)
			{
				// Heredoc
				ptr += 2;
				while (ft_isspace(*ptr))
					ptr++;
				// Extraire le délimiteur
				delim_end = ptr;
				while (*delim_end && !ft_isspace(*delim_end))
					delim_end++;
				redir->heredoc = ft_substr(ptr, 0, delim_end - ptr);
				ptr = delim_end;
			}
			else if (strncmp(ptr, ">>", 2) == 0)
			{
				// Append
				ptr += 2;
				while (ft_isspace(*ptr))
					ptr++;
				file_end = ptr;
				while (*file_end && !ft_isspace(*file_end))
					file_end++;
				redir->append_file = ft_substr(ptr, 0, file_end - ptr);
				ptr = file_end;
			}
			else if (*ptr == '<')
			{
				// Input redirection
				ptr++;
				while (ft_isspace(*ptr))
					ptr++;
				file_end = ptr;
				while (*file_end && !ft_isspace(*file_end))
					file_end++;
				redir->input_file = ft_substr(ptr, 0, file_end - ptr);
				ptr = file_end;
			}
			else if (*ptr == '>')
			{
				// Output redirection
				ptr++;
				while (ft_isspace(*ptr))
					ptr++;
				file_end = ptr;
				while (*file_end && !ft_isspace(*file_end))
					file_end++;
				redir->output_file = ft_substr(ptr, 0, file_end - ptr);
				ptr = file_end;
			}
		}
		ptr++;
	}
	return (redir);
}

int	handle_heredoc(char *delim)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

// fonction qui va parser les pipes :
// 1. fait un split pour separer les commandes par pipes
// 2. ensuite on parcours ces commandes et on creer une cellule pour chaque commande
// 3. on parser les redirections
// 4. on finit par stcoker la liste des pipes dans la cellule

void	parse_pipes(t_sep *cell)
{
	char	**pipe_commands;
	int		i;
	t_pip	*pipe_list;
	t_pip	*pipe_cell;
	t_pip	*current;

	pipe_list = NULL;
	pipe_commands = ft_split_pipes(cell->cmd_sep, '|');
	if (!pipe_commands)
		return ;
	i = 0;
	while (pipe_commands[i])
	{
		pipe_cell = malloc(sizeof(t_pip));
		if (!pipe_cell)
			return ;
		pipe_cell->cmd_pipe = ft_strdup(pipe_commands[i]);
		if (!pipe_cell->cmd_pipe)
		{
			free(pipe_cell);
			return ;
		}
		// Parser les redirections
		pipe_cell->redirection = parse_redirections(pipe_cell->cmd_pipe);
		pipe_cell->next = NULL;
		pipe_cell->prev = NULL;
		// Ajouter à la liste des pipes
		if (!pipe_list)
			pipe_list = pipe_cell;
		else
		{
			current = pipe_list;
			while (current->next)
				current = current->next;
			current->next = pipe_cell;
			pipe_cell->prev = current;
		}
		i++;
	}
	cell->pipcell = pipe_list;
	free_str_array(pipe_commands);
}
