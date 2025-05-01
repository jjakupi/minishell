/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:13:31 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 11:21:13 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_export_token(const char *str)
{
	char	*eq;
	char	*name;
	size_t	name_len;
	int		valid;

	if (str == NULL)
		return (0);
	eq = ft_strchr(str, '=');
	if (eq != NULL)
	{
		name_len = eq - str;
		name = ft_strndup(str, name_len);
		if (name == NULL)
			return (0);
		valid = is_valid_identifier(name);
		free(name);
		return (valid);
	}
	return (is_valid_identifier(str));
}

int	is_numeric(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
