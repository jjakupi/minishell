/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:24:35 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:46:42 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = (unsigned char) c;
		i++;
	}
	return (s);
}
/*
int main(void)
{
	char test1 [20];

	ft_memset(test1, 'L', sizeof(test1));
	ft_memset(test1, 'D', 10);
	printf("Test1 after ft_memset: %.*s\n", (int)sizeof(test1), test1);
	return (0);
}
*/
