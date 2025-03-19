/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:09:07 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:15:46 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (needle[0] == '\0')
		return ((char *)haystack);
	i = 0;
	while (haystack[i] != '\0' && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && i + j < len)
		{
			if (needle[j] == '\0')
				return ((char *)haystack + i);
			j++;
		}
		if (needle[j] == '\0')
			return ((char *)haystack + i);
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>

int main()
{
    const char *haystack = "je mange un appplepie with my friends";
    const char *needle = "with";
    size_t n = 50;

    char *result = ft_strnstr(haystack, needle, n);
    if (result != NULL)
        printf("Found: %s\n", result);
    else
        printf("Not found\n");

    return (0);
}*/
