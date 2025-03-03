/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:42:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/03/03 11:21:36 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	handle_heredoc_signal(int sig)
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

void	setup_parent_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	is_quoted_delimiter(char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"') ||
			(delimiter[0] == '\'' && delimiter[len - 1] == '\''))
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
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"') ||
			(delimiter[0] == '\'' && delimiter[len - 1] == '\''))
			return (ft_substr(delimiter, 1, len - 2));
	}
	return (ft_strdup(delimiter));
}
