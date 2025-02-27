/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:47:02 by jmaizel           #+#    #+#             */
/*   Updated: 2025/02/25 14:25:48 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_realloc(char *ptr, size_t old_size, size_t new_size)
{
	char	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size <= old_size)
		return (ptr);
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

char	*resize_buffer(char *result, size_t *size, size_t needed_size)
{
	char	*new_result;

	if (needed_size >= *size)
	{
		new_result = ft_realloc(result, *size, *size + needed_size + 1024);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		*size += needed_size + 1024;
		return (new_result);
	}
	return (result);
}
