/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 12:30:35 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:21:20 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return (((unsigned char *)s) + i);
		i++;
	}
	return (NULL);
}
/*
int main()
{
	const char *t = "Hello World";
	char *result;

	result = ft_memchr(t, 'H', 11);
	if (result != NULL)
	printf("Found the character at the the position: %ld\n", result - t);
	else
	printf("character not found\n");
	return (0);
}
*/				
