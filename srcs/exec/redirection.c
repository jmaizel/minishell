/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:33:23 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 16:53:02 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	handle_redirection(t_lexer *redir)
{
	if (redir->token.type == TOK_INPUT_REDIR)
		handle_input_redirection(redir);
	else if (redir->token.type == TOK_OUTPUT_REDIR)
		handle_output_redirection(redir);
	else if (redir->token.type == TOK_APPEND_REDIR)
		handle_append_redirection(redir);
	else if (redir->token.type == TOK_HEREDOC_REDIR)
		handle_heredoc(redir);
	else
	{
		ft_putendl_fd("Unsupported redirection type", STDERR_FILENO);
		exit(ERROR_EXIT);
	}
}

void	apply_redirections(t_simple_cmds *cmd)
{
	t_lexer	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		handle_redirection(redir);
		redir = redir->next;
	}
}

void	handle_input_redirection(t_lexer *redir)
{
	int	fd;

	fd = open(redir->token.value, O_RDONLY);
	if (fd == -1)
	{
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	handle_output_redirection(t_lexer *redir)
{
	int	fd;

	fd = open(redir->token.value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	handle_append_redirection(t_lexer *redir)
{
	int	fd;

	fd = open(redir->token.value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}
