/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 12:47:21 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	update_entry(char **env, int idx, const char *assignment)
{
	char	*new;

	new = ft_strdup (assignment);
	if (!new)
		return (1);
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
	size_t	len;

	plus = ft_strchr(arg, '+');
	if (plus == NULL)
		return (NULL);
	len = plus - arg;
	return (ft_substr(arg, 0, len));
}

char	*get_append_suffix(const char *arg)
{
	char	*plus;

	plus = ft_strchr(arg, '+');
	if (plus == NULL || plus[1] != '+')
		return (NULL);
	return (ft_strdup(plus + 2));
}

char	*build_appended_entry(char ***env, int idx,
			const char *key, const char *suffix)
{
	char	*oldval;
	char	*tmp;
	char	*prefix;
	char	*res;

	oldval = ft_strchr((*env)[idx], '=') + 1;
	tmp = ft_strjoin(oldval, suffix);
	if (tmp == NULL)
		return (NULL);
	prefix = ft_strjoin(key, "=");
	if (prefix == NULL)
	{
		free(tmp);
		return (NULL);
	}
	res = ft_strjoin(prefix, tmp);
	free(tmp);
	free(prefix);
	return (res);
}
