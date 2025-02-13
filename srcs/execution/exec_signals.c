/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:15:45 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/13 19:48:12 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static void	handle_parent_sigint(int sig)
{
	(void)sig;
	g_signal_received = 130;
}

static void	handle_parent_sigquit(int sig)
{
	(void)sig;
}

static void	handle_child_sigint(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
	g_signal_received = 130;
}

static void	handle_child_sigquit(int sig)
{
	(void)sig;
	g_signal_received = 131;
}

void	setup_parent_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = handle_parent_sigint;
	sa_quit.sa_handler = handle_parent_sigquit;
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = handle_child_sigint;
	sa_quit.sa_handler = handle_child_sigquit;
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
