/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:07:26 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:07:31 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}
/*
void	change_chars(unsigned int i, char *c)
{
	if (*c >= 'a' && *c <= 'z')
		*c = *c-32;
}
int main()
{
	char str[] = "Hello, World!";
	printf("before ft_striteri: %s\n", str);

	ft_striteri(str, change_chars);
	printf("after ft_striteri: %s\n", str);
	return (0);
}
*/
