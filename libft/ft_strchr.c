/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:06:53 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:06:56 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	i;

	i = (unsigned char)c;
	while (*s)
	{
		if (*s == i)
			return ((char *)s);
		s++;
	}
	if (i == '\0')
		return ((char *)s);
	return (NULL);
}
/*
int main() 
{
    const char *str = "Hello, World!";
    char c1 = 'S';
    char c2 = '\0'; // Testing for null terminator
   
    // C1
    char *result1 = ft_strchr(str, c1);
    if (result1 != NULL) 
    {
        printf("Found '%c' at position: %ld\n", c1, result1 - str);
    }
    else 
    {
        printf("Character '%c' not found.\n", c1);
    }
    
   // C2
    char *result2 = ft_strchr(str, c2);
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
