/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:06:18 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:06:21 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(-n, fd);
	}
	else if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + '0', fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
}
/*
int main ()
{
 	int num0 = -2147483648;
 	int num1 = 2147483647;
 	int num2 = 69;
 	ft_putnbr_fd(num0, 1);
	ft_printf("\n");
 	ft_putnbr_fd(num1, 1);
 	printf("\n");
 	ft_putnbr_fd(num2, 1);
 	printf("\n");
 	return (0);
}*/
