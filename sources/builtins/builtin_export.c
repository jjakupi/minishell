/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:08:44 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 12:52:21 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_export_append(char *arg, char ***env)
{
	char	*key;
	char	*suffix;
	int		idx;
	char	*entry;

	key = get_append_key(arg);
	if (key == NULL)
		return (1);
	suffix = get_append_suffix(arg);
	if (suffix == NULL)
		return (free(key), 1);
	idx = env_op(*env, key, 1);
	if (idx < 0)
		return (free(key), free(suffix), set_env_var(env, arg));
	entry = build_appended_entry(env, idx, key, suffix);
	free(key);
	free(suffix);
	if (entry == NULL)
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

static int	process_export_arg(char *arg, char ***env)
{
	char	*plus;

	plus = ft_strchr(arg, '+');
	if (plus != NULL && plus[1] == '=')
		return (handle_export_append(arg, env) != 0);
	return (handle_export_token(arg, env) != 0);
}

int	builtin_export(t_command *cmd, char ***env)
{
	int	i;
	int	status;

	if (cmd->arg_count == 0)
	{
		print_sorted_env(*env);
		return (0);
	}
	status = 0;
	i = 0;
	while (i < cmd->arg_count)
	{
		if (process_export_arg(cmd->args[i], env))
			status = 1;
		i++;
	}
	return (status);
}
