/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 10:31:04 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/29 10:36:40 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
#define TOOLS_H

# include "../libft/includes/libft.h"

char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);
char	*ft_charjoin(char *str, char c);
void	sort_string_array(char **array);

#endif