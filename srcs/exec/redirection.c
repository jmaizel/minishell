/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:00:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 21:07:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static void	open_and_redirect(int std_fd, char *file, int flags, mode_t mode)
{
	int	fd;

	if (!file)
		return ;
	fd = open(file, flags, mode);
	if (fd == -1)
	{
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, std_fd) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	handle_input_redirection(char *file)
{
	open_and_redirect(STDIN_FILENO, file, O_RDONLY, 0);
}

void	handle_output_redirection(char *file)
{
	open_and_redirect(STDOUT_FILENO, file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

void	handle_append_redirection(char *file)
{
	open_and_redirect(STDOUT_FILENO, file, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

static void	handle_redirection(t_parsed_cmd *cmd, t_pip *pip)
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
		handle_heredoc(cmd->heredoc_delim[i++], pip);
}

void	apply_redirections(t_pip *pip)
{
	if (pip && pip->redirection)
		handle_redirection(pip->redirection, pip);
}
