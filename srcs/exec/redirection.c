/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:33:23 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 20:33:10 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	handle_input_redirection(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
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

void	handle_output_redirection(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

void	handle_append_redirection(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

static void	handle_redirection(t_parsed_cmd *cmd, t_pip *pip, t_tools *tools)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (i < cmd->input_count)
		handle_input_redirection(cmd->input_file[i++]);
	i = 0;
	while (i < cmd->output_count)
		handle_output_redirection(cmd->output_file[i++]);
	i = 0;
	while (i < cmd->append_count)
		handle_append_redirection(cmd->append_file[i++]);
	i = 0;
	while (i < cmd->heredoc_count)
		handle_heredoc(cmd->heredoc_delim[i++], pip, tools);
}

void	apply_redirections(t_pip *pip, t_tools *tools)
{
	if (pip && pip->redirection)
		handle_redirection(pip->redirection, pip, tools);
}
