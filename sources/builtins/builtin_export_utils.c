/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:14:02 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 17:35:57 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_op(char **env, const char *key, int mode)
{
	int	i;
	int	key_len;

	i = 0;
	if (mode == 0)
	{
		while (env[i])
			i++;
		return (i);
	}
	else if (mode == 1)
	{
		key_len = ft_strlen(key);
		while (env[i])
		{
			if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
				return (i);
			i++;
		}
		return (-1);
	}
	return (-1);
}

int	is_valid_identifier_export(const char *token)
{
	if (!token || token[0] == '=' || ft_isdigit(token[0]))
		return (0);
	else if (ft_strchr(token, '\'') || ft_strchr(token, '"'))
		return (0);
	return (1);
}

int	cmpfunc(const void *a, const void *b)
{
	const char	*s1;
	const char	*s2;

	s1 = *(const char **)a;
	s2 = *(const char **)b;
	return ((int)strcmp(s1, s2));
}

char	**copy_env(char **env)
{
	int		n;
	char	**copy;
	int		i;

	n = env_op(env, NULL, 0);
	copy = malloc((n + 1) * sizeof(char *));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = env[i];
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

void	extract_key(const char *assignment, char *key)
{
	int		len;
	char	*eq;

	eq = ft_strchr(assignment, '=');
	if (eq)
		len = eq - assignment;
	else
		len = ft_strlen(assignment);
	if (len > 255)
		len = 255;
	ft_strlcpy(key, assignment, len + 1);
}
