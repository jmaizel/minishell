/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:31:48 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/17 11:41:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

void	execute_simple_command(t_simple_cmds *cmd, t_tools *tools);

void	apply_redirections(t_simple_cmds *cmd);
void	handle_input_redirection(t_lexer *redir);
void	handle_output_redirection(t_lexer *redir);
void	handle_append_redirection(t_lexer *redir);
void	handle_heredoc(t_lexer *redir);

char	*find_executable(char *command, char **env);

#endif