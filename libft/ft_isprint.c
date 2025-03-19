/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:05:03 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:14:20 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c < 127)
		return (1);
	return (0);
}
/*
int main (void)
{
	char s = 32;

	if(ft_isprint(s))
		printf("%d is printable\n", s);
	else
		printf("%d is not printable\n", s);
	return (0);
}*/
