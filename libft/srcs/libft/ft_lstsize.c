/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:03:47 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:52 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_lstsize(t_list *lst)
{
	int		count;
	t_list	*temp_list;

	count = 0;
	temp_list = lst;
	while (temp_list)
	{
		temp_list = temp_list->next;
		count++;
	}
	return (count);
}

/*
#include <stdio.h>

int main(void)
{
    t_list *node1 = ft_lstnew("Élément 1");
    t_list *node2 = ft_lstnew("Élément 2");
    t_list *node3 = ft_lstnew("Élément 3");

    node1->next = node2;
    node2->next = node3;

    printf("Taille de la liste: %d\n", ft_lstsize(node1));

    free(node1);
    free(node2);
    free(node3);

    return 0;
}
*/