/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 15:22:15 by jjakupi          ###   ########.fr       */
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

char *get_append_key(const char *arg)
{
    char *plus = ft_strchr(arg, '+');
    size_t len;

    if (!plus || plus[1] != '=')
        return NULL;
    len = plus - arg;
    char *key = malloc(len + 1);
    if (!key)
        return NULL;
    memcpy(key, arg, len);
    key[len] = '\0';
    return key;
}

char *get_append_suffix(const char *arg)
{
    char *plus = ft_strchr(arg, '+');
    if (!plus || plus[1] != '=')
        return NULL;
    return ft_strdup(plus + 2);
}
char *build_appended_entry(char ***env_ptr, int idx,
	const char *key, const char *suffix)
{
char **env = *env_ptr;
char *oldval = ft_strchr(env[idx], '=') + 1;
char *tmp = ft_strjoin(oldval, suffix);
if (!tmp)
return NULL;
char *eq = ft_strjoin(key, "=");
if (!eq)
{
free(tmp);
return NULL;
}
char *entry = ft_strjoin(eq, tmp);
free(tmp);
free(eq);
return entry;
}
