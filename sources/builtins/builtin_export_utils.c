/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:14:02 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/21 16:20:21 by julrusse         ###   ########.fr       */
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

void	print_sorted_env(char **env)
{
	int		count;
	char	**env_copy;
	int		i;
	char	*var;
	char	*eq;
	char	key[256];
	int		key_len;

	count = env_op(env, NULL, 0);
	env_copy = malloc((count + 1) * sizeof(char *));
	if (env_copy == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		env_copy[i] = env[i];
		i++;
	}
	env_copy[count] = NULL;
	qsort(env_copy, count, sizeof(char *), cmpfunc);
	i = 0;
	while (i < count)
	{
		var = env_copy[i];
		eq = strchr(var, '=');
		if (eq != NULL)
		{
			key_len = eq - var;
			if (key_len > 255)
				key_len = 255;
			strncpy(key, var, key_len);
			key[key_len] = '\0';
			if (*(eq + 1) == '\0')
				printf("declare -x %s\n", key);
			else
				printf("declare -x %s=\"%s\"\n", key, eq + 1);
		}
		else
		{
			printf("declare -x %s\n", var);
		}
		i++;
	}
	free(env_copy);
}
