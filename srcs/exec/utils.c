/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:30:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 21:11:34 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

char	*ft_charjoin(char *str, char c)
{
	char	*new;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new = malloc(len + 2);
	if (!new)
		return (NULL);
	ft_strlcpy(new, str, len + 1);
	new[len] = c;
	new[len + 1] = '\0';
	free(str);
	return (new);
}

char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	result = ft_strjoin(s1, s2);
	if (!result)
		return (NULL);
	if (free_s1 && s1)
		free(s1);
	if (free_s2 && s2)
		free(s2);
	return (result);
}
