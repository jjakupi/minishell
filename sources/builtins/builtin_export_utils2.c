/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 13:55:06 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sort_env(char **arr)
{
	int		n;
	int		i;
	char	*tmp;

	n = env_op(arr, NULL, 0);
	i = 0;
	while (i < n - 1)
	{
		if (ft_strcmp(arr[i], arr[i + 1]) > 0)
		{
			tmp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
}

void	print_entry(const char *var)
{
	char	*eq;
	char	key[256];
	int		key_len;

	eq = ft_strchr(var, '=');
	if (eq)
	{
		key_len = (int)(eq - var);
		if (key_len > 255)
			key_len = 255;
		ft_strlcpy(key, var, key_len + 1);
		if (*(eq + 1) == '\0')
			printf("declare -x %s\n", key);
		else
			printf("declare -x %s=\"%s\"\n", key, eq + 1);
	}
	else
		printf("declare -x %s\n", var);
}

void	print_sorted_env(char **env)
{
	char	**copy;
	int		i;

	copy = copy_env(env);
	if (copy == NULL)
		return ;
	sort_env(copy);
	i = 0;
	while (copy[i])
	{
		print_entry(copy[i]);
		i++;
	}
	free(copy);
}

int	add_entry(char ***env_ptr, const char *entry)
{
	char	**old_env;
	char	**new_env;
	int		n;
	int		i;

	old_env = *env_ptr;
	n = env_op(old_env, NULL, 0);
	new_env = malloc((n + 2) * sizeof(char *));
	if (new_env == NULL)
		return (1);
	i = 0;
	while (i < n)
	{
		new_env[i] = old_env[i];
		i++;
	}
	new_env[n] = ft_strdup(entry);
	if (new_env[n] == NULL)
		return (free(new_env), 1);
	new_env[n + 1] = NULL;
	free(old_env);
	*env_ptr = new_env;
	return (0);
}
