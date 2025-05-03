/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:08:44 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 12:32:43 by julrusse         ###   ########.fr       */
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

static int	handle_export_append(char *arg, char ***env)
{
	char	*plus;
	char	key[256];
	char	*suffix;
	char	*oldval;
	char	*tmp;
	char	*entry;
	int		idx;

	plus = ft_strchr(arg, '+');
	ft_strlcpy(key, arg, plus - arg + 1);
	suffix = plus + 2;
	idx = env_op(*env, key, 1);
	if (idx < 0)
		return (set_env_var(env, arg));
	oldval = ft_strchr((*env)[idx], '=') + 1;
	tmp = ft_strjoin(oldval, suffix);
	if (!tmp)
		return (1);
	entry = ft_strjoin(key, "=");
	if (!entry)
	{
		free(tmp);
		return (1);
	}
	oldval = entry;
	entry = ft_strjoin(oldval, tmp);
	free(tmp);
	free(oldval);
	if (!entry)
		return (1);
	update_entry(*env, idx, entry);
	free(entry);
	return (0);
}

static int	handle_export_token(char *arg, char ***env)
{
	if (!is_valid_export_token(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	if (set_env_var(env, arg) != 0)
		return (1);
	return (0);
}

int	builtin_export(t_command *cmd, char ***env)
{
	int		i;
	int		status;
	char	*arg;
	char	*plus;

	status = 0;
	if (cmd->arg_count == 0)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 0;
	while (i < cmd->arg_count)
	{
		arg = cmd->args[i];
		plus = ft_strchr(arg, '+');
		if (plus != NULL && plus[1] == '=')
		{
			if (handle_export_append(arg, env) != 0)
				status = 1;
		}
		else
		{
			if (handle_export_token(arg, env) != 0)
				status = 1;
		}
		i++;
	}
	return (status);
}
