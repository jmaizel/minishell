/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:18:02 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:36 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*temp_list;

	temp_list = lst;
	while (temp_list)
	{
		f(temp_list->content);
		temp_list = temp_list->next;
	}
}

/*
#include <stdio.h>

void apply_function(void *content)
{
    printf("Content: %s\n", (char *)content);
}

int main(void)
{
    t_list *node1 = ft_lstnew("Hello");
    t_list *node2 = ft_lstnew("World");
    t_list *node3 = ft_lstnew("42");

    ft_lstadd_back(&node1, node2);
    ft_lstadd_back(&node1, node3);

    printf("Itérer à travers la liste:\n");
    ft_lstiter(node1, apply_function);

    t_list *temp;
    while (node1)
    {
        temp = node1->next;
        free(node1); 
        node1 = temp;
    }
    return 0;
}
*/