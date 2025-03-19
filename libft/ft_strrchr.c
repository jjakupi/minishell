/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:09:22 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:09:24 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	if (c == '\0')
		return ((char *)s + i);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i--;
	}
	return (NULL);
}
/*
#include <stdio.h>

int main()
{
    const char *str = "Hello, World!";
    char c1 = 'l';
    char c2 = '\0';
	
    // C1
    char *result1 = ft_strrchr(str, c1);
    if (result1 != NULL)
    {
        printf("Found '%c' at position: %ld\n", c1, result1 - str);
    }
    else
    {
        printf("Character '%c' not found.\n", c1);
    }

   // C2
    char *result2 = ft_strrchr(str, c2);
    if (result2 != NULL)
    {
        printf("Found null terminator at position: %ld\n", result2 - str);
    }
    else
    {
        printf("Character '%c' not found.\n", c2);
    }
        return (0);
}
*/
