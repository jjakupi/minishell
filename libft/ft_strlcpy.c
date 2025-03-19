/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:08:13 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:08:15 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		while (src[i] != '\0')
			i++;
		return (i);
	}
	while (i < size - 1 && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	if (i < size)
		dest[i] = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}
/*
#include <stdio.h>

int main(void)
{
    const char src[] = "Hello, World!";

    char dest[2]; 

    size_t left = ft_strlcpy(dest, src, sizeof(dest));

    printf("Source string: %s\n", src);
    printf("Destination string after ft_strlcpy: %s\n", dest);
    printf("Length of the source string: %zu\n", left);

    return (0);
}
*/
