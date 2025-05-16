/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:28:27 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/16 11:54:10 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

int	env_idx(char **env, const char *key)
{
	int		i;
	int		key_len;

	i = 0;
	key_len = (int)ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0
			&& (env[i][key_len] == '=' || env[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	env_remove_var(char ***env_ptr, const char *key)
{
	char	**env;
	int		idx;
	int		len;
	int		i;

	env = *env_ptr;
	idx = env_idx(env, key);
	if (idx == -1)
		return (0);
	len = env_len(env);
	i = idx;
	while (i < len - 1)
	{
		env[i] = env[i + 1];
		i++;
	}
	env[len - 1] = NULL;
	return (0);
}

int	valid_unset_identifier(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(t_command *cmd, char ***env)
{
	int		i;
	int		ret;
	char	*name;

	ret = EXIT_SUCCESS;
	if (cmd->arg_count == 0)
		return (ret);
	i = 0;
	while (i < cmd->arg_count)
	{
		name = cmd->args[i];
		if (!valid_unset_identifier(name))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(name, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = EXIT_FAILURE;
		}
		else
			env_remove_var(env, name);
		i++;
	}
	return (ret);
}
