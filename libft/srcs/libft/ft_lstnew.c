/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:55:52 by cdedessu          #+#    #+#             */
/*   Updated: 2024/11/05 21:30:47 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*res;

	res = (t_list *) malloc(sizeof(t_list));
	if (!res)
		return (NULL);
	res->content = content;
	res->next = NULL;
	return (res);
}

/*
#include <stdio.h>

int main(void)
{
    char *content = "Hello, world!";
    t_list *new_node;
    new_node = ft_lstnew(content);

    if (new_node == NULL)
    {
        printf("Erreur : la création du nœud a échoué.\n");
        return (1);
    }

    printf("Contenu du nœud : %s\n", (char *)new_node->content);
    free(new_node);
    return (0);
}
*/