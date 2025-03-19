/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:03:20 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:21:29 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	unsigned int	ft_count_lennr(int number)
{
	unsigned int	len;

	len = 0;
	if (number <= 0)
		len = 1;
	while (number != 0)
	{
		number /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*str;
	unsigned int	number;
	unsigned int	len;

	len = ft_count_lennr(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		number = -n;
	}
	else
		number = n;
	if (n == 0)
		str[0] = '0';
	str[len] = '\0';
	while (number != 0)
	{
		str[len - 1] = (number % 10) + '0';
		number = number / 10;
		len--;
	}
	return (str);
}
/*
int main()
{
	int num = 0;
	//int num = -1;
 	// int num = 2147483647;
	//int num = -2147483647;
 	printf("Len: %d\n", ft_count_lennr(num));
 	printf("Output: %s\n",  ft_itoa(num));
}*/
