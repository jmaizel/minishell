/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:34:50 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:30 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp_list;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp_list = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp_list;
	}
	*lst = NULL;
}

/*
#include <stdio.h>

void del_function(void *content)
{
    printf("Suppression du contenu : %s\n", (char *)content);
}

int main(void)
{
    t_list *head;
    t_list *node1;
    t_list *node2;
    t_list *node3;

    node1 = ft_lstnew("Premier");
    node2 = ft_lstnew("Deuxième");
    node3 = ft_lstnew("Troisième");

    node1->next = node2;
    node2->next = node3;

    head = node1;

    printf("Avant suppression :\n");
    t_list *temp_list = head;
    while (temp_list)
    {
        printf("Contenu du nœud : %s\n", (char *)temp_list->content);
        temp_list = temp_list->next;
    }

    ft_lstclear(&head, del_function);

    if (head == NULL)
        printf("Liste vide après suppression.\n");
    return 0;
}
*/