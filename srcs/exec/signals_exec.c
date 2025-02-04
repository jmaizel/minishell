/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:10:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/04 19:45:50 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	sigint_handler(int sig)
{
	(void)sig;
	if (write(STDOUT_FILENO, "\nminishell> ", 12) == -1)
		return ;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_sigint_handler(int sig)
{
	(void)sig;
	if (sig == SIGINT && write(STDOUT_FILENO, "\n", 1) == -1)
		return ;
}

void	setup_signals_exec(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = child_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_DFL);
}
