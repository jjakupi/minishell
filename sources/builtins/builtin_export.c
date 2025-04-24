/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:08:44 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/24 12:20:46 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	add_entry(char ***env_ptr, const char *entry)
{
	char	**env;
	int		n;

	env = *env_ptr;
	n = env_op(env, NULL, 0);
	env = realloc(env, (n + 2) * sizeof(char *));
	if (!env)
		return (1);
	env[n] = ft_strdup(entry);
	if (!env[n])
		return (1);
	env[n + 1] = NULL;
	*env_ptr = env;
	return (0);
}

int	update_entry(char **env, int idx, const char *assignment)
{
	char	*new;

	new = ft_strdup(assignment);
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

int	builtin_export(t_command *cmd, char ***env)
{
	int	i;
	int	status;

	status = 0;
	if (cmd->arg_count == 0)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 0;
	while (i < cmd->arg_count)
	{
		if (!is_valid_export_token(cmd->args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else if (set_env_var(env, cmd->args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
