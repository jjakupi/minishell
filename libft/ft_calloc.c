/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:03:50 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:13:26 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*array;

	array = malloc(nmemb * size);
	if (!array)
		return (NULL);
	else
		ft_bzero(array, (nmemb * size));
	return (array);
}
/*
int main(void)
{
	int	*array;

	array = (int*)ft_calloc(0, sizeof(int));
	//array = (int*)ft_calloc(100000000000, sizeof(int));
	if (array == NULL)
		printf("Memory not allocated \n");
	else
		printf("Memory allocated successfully \n");
	free(array);
	return (0);
}*/
