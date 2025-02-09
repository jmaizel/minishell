/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:24:33 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:40 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*temp_list;

	if (!lst)
		return (NULL);
	temp_list = lst;
	while (temp_list->next)
		temp_list = temp_list->next;
	return (temp_list);
}

/*
#include <stdio.h>

int main(void)
{
    t_list *node1;
    t_list *node2;
    t_list *node3;
    t_list *last_node;

    node1 = ft_lstnew("Premier");
    node2 = ft_lstnew("Deuxième");
    node3 = ft_lstnew("Troisième");

    node1->next = node2;
    node2->next = node3;
	
    last_node = ft_lstlast(node1);

    if (last_node)
        printf("Le contenu du dernier nœud est : %s\n", 
				(char *)last_node->content);
    else
        printf("La liste est vide.\n");

    free(node1);
    free(node2);
    free(node3);

    return 0;
}
*/