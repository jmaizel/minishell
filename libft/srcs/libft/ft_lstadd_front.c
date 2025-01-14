/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 14:04:28 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:26 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

/*
#include <stdio.h>

int main(void)
{

    t_list *head;
    t_list *new_node1; 
    t_list *new_node2; 

    new_node1 = ft_lstnew("Second node");
    head = new_node1;

    new_node2 = ft_lstnew("First node");

    ft_lstadd_front(&head, new_node2);

    printf("Contenu du premier nœud : %s\n", (char *)head->content);
    printf("Contenu du deuxième nœud : %s\n", (char *)head->next->content);

    free(new_node1);
    free(new_node2);
    return (0);
}
*/