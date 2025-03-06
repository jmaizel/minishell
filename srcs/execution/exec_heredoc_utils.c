/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:26:10 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/04 21:36:29 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	setup_child_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_heredoc_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

int	is_quoted_delimiter(char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\''))
			return (1);
	}
	return (0);
}

char	*remove_quotes(char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\''))
			return (ft_substr(delimiter, 1, len - 2));
	}
	return (ft_strdup(delimiter));
}

char	*expand_heredoc_line(char *line, t_tools *tools)
{
	char	*expanded;
	char	*exit_str;

	if (!ft_strchr(line, '$'))
		return (ft_strdup(line));
	if (ft_strcmp(line, "$?") == 0)
	{
		exit_str = ft_itoa(tools->exit_code);
		return (exit_str);
	}
	expanded = expand_str(line, tools);
	if (!expanded)
		return (ft_strdup(line));
	return (expanded);
}
