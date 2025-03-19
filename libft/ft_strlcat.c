/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:07:59 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:08:02 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[i] && i < dstsize)
		i++;
	while (src[j] && (i + j + 1) < dstsize)
	{
		dest[i + j] = src[j];
		j++;
	}
	if (i < dstsize)
		dest[i + j] = '\0';
	while (src[j])
		j++;
	return (i + j);
}
/*
#include <stdio.h>

int main() 
{
    char dst[10] = "Hi ";
    const char *src = "Hell:(o World";

    size_t newLength = ft_strlcat(dst, src, sizeof(dst));

    printf("Resulting string: '%s'\n", dst);
    printf("Total length returned: %zu\n", newLength);

    return 0;
}*/
