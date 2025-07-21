/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:16:37 by jmaizel           #+#    #+#             */
/*   Updated: 2025/03/11 13:18:40 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	update_exit_status(int status, int *last_exit_status)
{
	if (WIFEXITED(status))
		*last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			*last_exit_status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			*last_exit_status = 131;
		else
			*last_exit_status = 128 + WTERMSIG(status);
	}
}
