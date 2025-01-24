/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:16:56 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/22 16:50:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h> 
# include <stdlib.h>
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 19
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

size_t	ft_strlen_v2(const char *s);
size_t	ft_strlcpy_v2(char *dst, const char *src, size_t size);
char	*ft_strchr_v2(const char *s, int c);
char	*ft_strjoin_v2(char const *s1, char const *s2);
char	*ft_strdup(const char *src);
char	*get_next_line(int fd);

#endif