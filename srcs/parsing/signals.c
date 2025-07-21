/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:58:53 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/11 12:21:14 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			g_signal_received = 0;

static void	handle_interactive_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_interactive_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

char	*get_user_input(void)
{
	char		*input;
	static int	eof_count = 0;

	if (g_signal_received)
	{
		eof_count = 0;
		g_signal_received = 0;
	}
	input = readline("minishell$ ");
	if (!input)
	{
		eof_count++;
		if (eof_count >= 1)
			return (NULL);
		return (ft_strdup(""));
	}
	eof_count = 0;
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

void	setup_exec_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	restore_signals(void)
{
	setup_interactive_signals();
}
