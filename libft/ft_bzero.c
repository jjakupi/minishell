/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bzero.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:30:15 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:13:13 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}
/*
int main(void)
{
    char test2[10] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A','A'};
    int i;
	
	printf("Test before ft_bzero: %s \n", test2);

	ft_bzero(test2, 5);

    printf("Test after ft_bzero:");

    i = 0;
    while (i < 10)
    {
        if (test2[i] == 0)
            printf("0");
	else
            printf("%c", test2[i]);
        i++;
    }

    printf("\n");
    return 0;
}
*/
