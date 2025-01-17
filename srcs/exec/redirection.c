/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:33:23 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 11:46:16 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	apply_redirections(t_simple_cmds *cmd)
{
	t_lexer	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->token.type == TOK_INPUT_REDIR)
			handle_input_redirection(redir);
		else if (redir->token.type == TOK_OUTPUT_REDIR)
			handle_output_redirection(redir);
		else if (redir->token.type == TOK_APPEND_REDIR)
			handle_append_redirection(redir);
		else if (redir->token.type == TOK_HEREDOC_REDIR)
			handle_heredoc(redir);
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

void	handle_heredoc(t_lexer *redir)
{
	int		fd;
	char	*line;

	fd = open("/tmp/.heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, redir->token.value, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	fd = open("/tmp/.heredoc_tmp", O_RDONLY);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		perror("heredoc failed");
		exit(EXIT_FAILURE);
	}
	close(fd);
	unlink("/tmp/.heredoc_tmp");
}
