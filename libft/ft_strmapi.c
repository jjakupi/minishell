/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:08:36 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:08:37 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	i = 0;
	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	while (s[i] != '\0')
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*1
char	change_str(unsigned int i, char c)
{
	if(c == 'H') return 'J';
	return (c);
	
}
2
char	change_str(unsigned int i, char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c+32);
	return(c);
}

int main ()
{
	char *original = "HELLO WORLD!";
	char *modified ;
	modified = ft_strmapi(original, change_str);
	if (modified)
	{
	printf("original: %s\n", original);
	printf("modified: %s\n", modified);
	free(modified);
	}
	return (0);
}*/
