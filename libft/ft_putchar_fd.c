/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:05:47 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:05:51 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*
int main ()
{
	int	i;
	i = 0;
	char	str[] = "Hello world!";
	while (str[i] != '\0')
	ft_putchar_fd(str[i++], 1);
	ft_putchar_fd('\n', 1);
	return (0);
}
*/
