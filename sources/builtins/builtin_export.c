/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:08:44 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 14:06:02 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	append_to_existing(char ***env, const char *key,
				const char *suffix)
{
	char	*entry;
	int		idx;
	int		ret;

	idx = env_op(*env, key, 1);
	entry = build_appended_entry(env, idx, (char *)key, (char *)suffix);
	if (entry == NULL)
		return (1);
	update_entry(*env, idx, entry);
	free(entry);
	ret = 0;
	return (ret);
}

static int	add_new_appended(char ***env, const char *key, const char *suffix)
{
	char	*eq;
	char	*entry;
	int		ret;

	eq = ft_strjoin(key, "=");
	if (eq == NULL)
		return (1);
	entry = ft_strjoin(eq, suffix);
	free(eq);
	if (entry == NULL)
		return (1);
	ret = set_env_var(env, entry);
	free(entry);
	return (ret);
}

static int	handle_export_append(char *arg, char ***env)
{
	char	*key;
	char	*suffix;
	int		idx;
	int		ret;

	key = get_append_key(arg);
	if (key == NULL)
		return (1);
	suffix = get_append_suffix(arg);
	if (suffix == NULL)
	{
		free(key);
		return (1);
	}
	idx = env_op(*env, key, 1);
	if (idx >= 0)
		ret = append_to_existing(env, key, suffix);
	else
		ret = add_new_appended(env, key, suffix);
	free(key);
	free(suffix);
	return (ret);
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
	return (set_env_var(env, arg) != 0);
}

int	builtin_export(t_command *cmd, char ***env)
{
	int		status;
	int		i;
	char	*arg;
	char	*plus;

	status = 0;
	if (cmd->arg_count == 0)
		return (print_sorted_env(*env), 0);
	i = 0;
	while (i < cmd->arg_count)
	{
		arg = cmd->args[i];
		plus = ft_strchr(arg, '+');
		if (plus != NULL && plus[1] == '=')
		{
			if (handle_export_append(arg, env))
				status = 1;
		}
		else if (handle_export_token(arg, env))
			status = 1;
		i++;
	}
	return (status);
}
