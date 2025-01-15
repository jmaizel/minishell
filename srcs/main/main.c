/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaizel <jmaizel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:20:12 by jmaizel           #+#    #+#             */
/*   Updated: 2025/01/15 12:33:13 by jmaizel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	char *test1 = "echo \"Hello 'world'\""; // Toutes les quotes sont fermées
	char *test2 = "echo 'Hello world";      // Single quote non fermée
	char *test3 = "echo \"Hello world";     // Double quote non fermée

	printf("Test 1: %d\n", is_quote_closed(test1)); // 1
	printf("Test 2: %d\n", is_quote_closed(test2)); // 0
	printf("Test 3: %d\n", is_quote_closed(test3)); // 0

	return (0);
}
