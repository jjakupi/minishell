/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 14:10:41 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	update_entry(char **env, int idx, const char *assignment)
{
	char	*new;

	new = ft_strdup (assignment);
	if (!new)
		return (1);
	free(env[idx]);
	env[idx] = new;
	return (0);
}

int	set_env_var(char ***env_ptr, const char *assignment)
{
	char	*eq;
	char	key[256];
	char	**env;
	int		idx;

	env = *env_ptr;
	eq = ft_strchr(assignment, '=');
	extract_key(assignment, key);
	idx = env_op(env, key, 1);
	if (!eq)
	{
		if (idx == -1)
			return (add_entry(env_ptr, key));
		return (0);
	}
	if (idx != -1)
		return (update_entry(env, idx, assignment));
	return (add_entry(env_ptr, assignment));
}

char	*get_append_key(const char *arg)
{
	char	*plus;
	char	*key;
	size_t	len;

	plus = ft_strchr(arg, '+');
	if (plus == NULL || plus[1] != '=')
		return (NULL);
	len = plus - arg;
	key = malloc(len + 1);
	if (key == NULL)
		return (NULL);
	ft_memcpy(key, arg, len);
	key[len] = '\0';
	return (key);
}

char	*get_append_suffix(const char *arg)
{
	char	*plus;

	plus = ft_strchr(arg, '+');
	if (!plus || plus[1] != '=')
		return (NULL);
	return (ft_strdup(plus + 2));
}

char	*build_appended_entry(char ***env_ptr, int idx,
			const char *key, const char *suffix)
{
	char	**env;
	char	*oldval;
	char	*tmp;
	char	*eq;
	char	*entry;

	env = *env_ptr;
	oldval = ft_strchr(env[idx], '=') + 1;
	tmp = ft_strjoin(oldval, suffix);
	if (tmp == NULL)
		return (NULL);
	eq = ft_strjoin(key, "=");
	if (eq == NULL)
	{
		free(tmp);
		return (NULL);
	}
	entry = ft_strjoin(eq, tmp);
	free(tmp);
	free(eq);
	return (entry);
}
