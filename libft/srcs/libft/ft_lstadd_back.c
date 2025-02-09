/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:04:15 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:21 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp_list;

	if (!lst || !new)
		return ;
	temp_list = ft_lstlast(*lst);
	if (!temp_list)
	{
		*lst = new;
		return ;
	}
	temp_list->next = new;
}
/*
#include <stdio.h>

int main(void)
{
    t_list *head;
    t_list *node1;
    t_list *node2;
    t_list *new_node;

    node1 = ft_lstnew("Premier nœud");
    node2 = ft_lstnew("Deuxième nœud");

    head = node1;
    node1->next = node2;

    new_node = ft_lstnew("Nœud ajouté à la fin");

    ft_lstadd_back(&head, new_node);

    t_list *temp = head;
    while (temp)
    {
        printf("Contenu du nœud : %s\n", (char *)temp->content);
        temp = temp->next;
    }

    free(node1);
    free(node2);
    free(new_node);
    return (0);
}
*/