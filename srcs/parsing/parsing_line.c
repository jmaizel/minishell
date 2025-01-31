/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:30:33 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/31 16:11:53 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_parsed_command(t_parsed_cmd *cmd)
{
	int	j;
	int	k;
	int	l;
	int	i;

	printf(" parsing results:\n");
	printf(" Full command: [%s]\n", cmd->full_cmd);
	if (cmd->input_count > 0)
	{
		printf(" Input File:\n");
		j = 0;
		while (j < cmd->input_count)
		{
			printf(" %d: [%s]\n", j + 1, cmd->input_file[j]);
			j++;
		}
	}
	if (cmd->output_count > 0)
	{
		printf(" Output File:\n");
		k = 0;
		while (k < cmd->output_count)
		{
			printf(" %d: [%s]\n", k + 1, cmd->output_file[k]);
			k++;
		}
	}
	if (cmd->append_count > 0)
	{
		printf(" Append File:\n");
		l = 0;
		while (l < cmd->append_count)
		{
			printf(" %d: [%s]\n", l + 1, cmd->append_file[l]);
			l++;
		}
	}
	if (cmd->heredoc_count > 0)
	{
		printf(" Heredoc Delimiters:\n");
		i = 0;
		while (i < cmd->heredoc_count)
		{
			printf(" %d: [%s]\n", i + 1, cmd->heredoc_delim[i]);
			i++;
		}
	}
}

void	free_parsed_cmd(t_parsed_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	free(cmd->full_cmd);
	free(cmd->cmd);
	i = 0;
	while (i < cmd->input_count)
	{
		free(cmd->input_file[i]);
		i++;
	}
	free(cmd->input_file);
	i = 0;
	while (i < cmd->output_count)
	{
		free(cmd->output_file[i]);
		i++;
	}
	free(cmd->output_file);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		free(cmd->heredoc_delim[i]);
		i++;
	}
	i = 0;
	while (i < cmd->append_count)
	{
		free(cmd->append_file[i]);
		i++;
	}
	free(cmd->heredoc_delim);
	free(cmd);
}

void	parsing_line(char *user_input, t_tools *tools)
{
	t_sep *sep_cell;
	t_pip *current_pipe;

	(void)tools;

	// Vérifie si la commande est valide avant de parser
	if (!is_valid_command(user_input))
	{
		printf("Erreur de syntaxe : commande invalide !\n");
		return;
	}

	// Crée une cellule de séparation contenant la ligne d'entrée
	sep_cell = create_cell(user_input);
	if (!sep_cell)
		return;

	// Parse les pipes et stocke dans sep_cell->pipcell
	parse_pipes(sep_cell);

	// Affiche chaque commande et ses redirections
	current_pipe = sep_cell->pipcell;
	while (current_pipe)
	{
		printf("\n=== PIPE SEGMENT ===\n");
		printf(" Command: [%s]\n", current_pipe->cmd_pipe);
		print_parsed_command(current_pipe->redirection);
		current_pipe = current_pipe->next;
	}

	// Nettoyage mémoire
	// (Pense à une fonction free_sep_cell si nécessaire)
}

