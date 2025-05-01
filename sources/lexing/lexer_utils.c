/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:22:15 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/25 16:26:44 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_with_error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	syntax_error(const char *unexpected_token)
{
	printf("minishell: syntax error near unexpected token `%s'\n",
		unexpected_token);
	return (1);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = 0;
	while (len < n && s[len])
		len++;
	dup = malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*start;

	start = dest;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (start);
}

char	*append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	if (str != NULL)
		len = ft_strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (!new_str)
		exit_with_error("malloc (append_char)");
	if (str != NULL)
	{
		ft_strcpy(new_str, str);
		free(str);
	}
	else
		new_str[0] = '\0';
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}
