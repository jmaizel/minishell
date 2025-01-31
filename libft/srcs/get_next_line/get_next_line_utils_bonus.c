/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:16:53 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 10:11:59 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line_bonus.h"

char	*gnl_strchr(const char *str, int c)
{
	int		i;
	char	copy_c;

	i = 0;
	copy_c = (char)c;
	while (str[i])
	{
		if (str[i] == copy_c)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == copy_c)
		return ((char *)&str[i]);
	return (NULL);
}

char	*gnl_strdup(const char *src)
{
	int		src_size;
	char	*copy;
	int		i;

	src_size = gnl_strlen(src);
	copy = (char *)malloc((src_size + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < src_size)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*gnl_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (gnl_strdup(""));
	if (!s1)
		return (gnl_strdup(s2));
	if (!s2)
		return (gnl_strdup(s1));
	len1 = gnl_strlen(s1);
	len2 = gnl_strlen(s2);
	res = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = -1;
	while (s1[++i])
		res[i] = s1[i];
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

size_t	gnl_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*gnl_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = gnl_strlen(s);
	if (start >= s_len)
		return (gnl_strdup(""));
	if (len > (s_len - start))
		len = s_len - start;
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
