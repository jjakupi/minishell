/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:45:30 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:45:33 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*d;
	char	*s;

	i = 0;
	if (!dest && !src)
		return (NULL);
	d = (char *)dest;
	s = (char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
/*
int main(void)
{
    char src[] = "Hello, World!";
    char dest[20];
    
    ft_memcpy(dest, src, 3);
    
    printf("Source: %s\n", src);
    printf("Destination: %s\n", dest);
    
    return (0);
}
*/
