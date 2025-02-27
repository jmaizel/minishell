/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:30:54 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:43 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*temp_list;

	if (!lst || !f || !del)
		return (NULL);
	res = NULL;
	while (lst)
	{
		temp_list = ft_lstnew(f(lst->content));
		if (!temp_list)
		{
			ft_lstclear(&res, del);
			return (NULL);
		}
		ft_lstadd_back(&res, temp_list);
		lst = lst->next;
	}
	return (res);
}

/*
#include <stdio.h>

void *double_content(void *content)
{
    int *new_value = malloc(sizeof(int));
    if (new_value)
        *new_value = (*(int *)content) * 2;
    return new_value;
}

void delete_content(void *content)
{
    free(content);
}

int main(void)
{
    int a = 1, b = 2, c = 3;
    t_list *lst = ft_lstnew(&a);
    t_list *lst2 = ft_lstnew(&b);
    t_list *lst3 = ft_lstnew(&c);

    ft_lstadd_back(&lst, lst2);
    ft_lstadd_back(&lst, lst3);

    printf("Liste originale:\n");
    t_list *temp = lst;
    while (temp)
    {
        printf("%d -> ", *(int *)(temp->content));
        temp = temp->next;
    }
    printf("NULL\n");

    t_list *new_list = ft_lstmap(lst, double_content, delete_content);
	
    printf("Nouvelle liste (contenu doublé):\n");
    temp = new_list;
    while (temp)
    {
        printf("%d -> ", *(int *)(temp->content));
        temp = temp->next;
    }
    printf("NULL\n");

    ft_lstclear(&lst, NULL);
    ft_lstclear(&new_list, delete_content);
    return 0;
}
*/