/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:04:29 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/27 16:05:39 by jmaizel          ###   ########.fr       */
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
// 1. on initialise tout les champs
// 2. on recherche si cest une entree ou une sortie
// 3. on parse les entree
// 4. on parse les sorties , pour les sorties ,
// d abord je regarde si c est un append avec >> et apres si c est une simple sortie
t_redirection	*parse_redirections(char *cmd)
{
	t_redirection	*redir;
	char			*input_redir;
	char			*output_redir;

	redir = malloc(sizeof(t_redirection));
	redir->input_file = NULL;
	redir->output_file = NULL;
	redir->append_file = NULL;
	redir->heredoc = NULL;
	input_redir = ft_strchr(cmd, '<');
	output_redir = ft_strchr(cmd, '>');
	if (input_redir)
	{
		redir->type = TOK_PIPE_IN;
		redir->input_file = extract_redirection_file(input_redir);
	}
	if (output_redir)
	{
		if (*(output_redir + 1) == '>')
		{
			redir->type = TOK_REDIRECTION;
			redir->output_file = extract_redirection_file(output_redir + 1);
		}
		else
		{
			redir->type = TOK_PIPE_OUT;
			redir->output_file = extract_redirection_file(output_redir);
		}
	}
	return (redir);
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
		pipe_cell->redirection = parse_redirections(pipe_cell->cmd_pipe);
		pipe_cell->next = NULL;
		pipe_cell->prev = NULL;
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
