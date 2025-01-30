/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:00:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/30 16:00:13 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tools.h"

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
	if (free_s1)
		free(s1);
	if (free_s2)
		free(s2);
	return (result);
}

void	sort_string_array(char **array)
{
	int		i;
	int		j;
	char	*temp;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			if (ft_strncmp(array[i], array[j], 100) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	free_tools(t_tools *tools)
{
	if (!tools)
		return;
	if (tools->env)
	{
		free_env(tools->env);
		tools->env = NULL;
	}
	free(tools);
	tools = NULL;
}

