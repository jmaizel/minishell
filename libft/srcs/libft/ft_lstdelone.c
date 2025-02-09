/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:25:36 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:33 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}
/*
void del_function(void *content)
{
    printf("Suppression du contenu : %s\n", (char *)content);
}

#include <stdio.h>

int main(void)
{
  
    t_list *node;

    node = ft_lstnew("Contenu du nœud");
    if (node)
        printf("Avant suppression, contenu du nœud : %s\n", 
				(char *)node->content);
    ft_lstdelone(node, del_function);
    return 0;
}
*/