/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:18:32 by jjakupi           #+#    #+#             */
/*   Updated: 2024/10/18 11:18:34 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countword(const char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

static char	*ft_strndup(const char *str, size_t n)
{
	char	*s;

	s = (char *)malloc(sizeof(char) * (n + 1));
	if (!s)
		return (NULL);
	ft_strlcpy(s, str, n + 1);
	return (s);
}

static int	ft_addwords(char **tab, const char *str, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		j = i;
		while (str[i] && str[i] != c)
			i++;
		if (i > j)
		{
			tab[k] = ft_strndup(str + j, i - j);
			if (!tab[k])
				return (0);
			k++;
		}
	}
	tab[k] = NULL;
	return (1);
}

static char	**ft_freetab(char **tab, int k)
{
	while (k >= 0)
	{
		free(tab[k]);
		k--;
	}
	free(tab);
	return (NULL);
}

char	**ft_split(const char *str, char c)
{
	char	**tab;
	int		count;
	int		addwords;

	if (!str)
		return (NULL);
	count = ft_countword(str, c);
	tab = (char **)malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (NULL);
	addwords = ft_addwords(tab, str, c);
	if (!addwords)
		return (ft_freetab(tab, count));
	return (tab);
}
