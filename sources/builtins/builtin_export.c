/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:08:44 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 19:09:30 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int handle_export_append(char *arg, char ***env)
{
    char *key = get_append_key(arg);
    if (!key)
        return 1;
    char *suffix = get_append_suffix(arg);
    if (!suffix)
        return (free(key), 1);

    int idx = env_op(*env, key, 1);
    if (idx >= 0)
    {
        char *entry = build_appended_entry(env, idx, key, suffix);
        free(key);
        free(suffix);
        if (!entry)
            return 1;
        update_entry(*env, idx, entry);
        free(entry);
        return 0;
    }

    // idx < 0: variable not present, create new KEY=suffix entry
    {
        char *eq = ft_strjoin(key, "=");
        if (!eq)
        {
            free(key);
            free(suffix);
            return 1;
        }
        char *new_entry = ft_strjoin(eq, suffix);
        free(eq);
        free(key);
        free(suffix);
        if (!new_entry)
            return 1;
        int ret = set_env_var(env, new_entry);
        free(new_entry);
        return ret;
    }
}

static int handle_export_token(char *arg, char ***env)
{
	if (!is_valid_export_token(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
	return 1;
    }
    return (set_env_var(env, arg) != 0);
}

int builtin_export(t_command *cmd, char ***env)
{
    int status = 0;
    if (cmd->arg_count == 0)
    {
        print_sorted_env(*env);
        return 0;
    }
    for (int i = 0; i < cmd->arg_count; i++)
    {
        char *arg = cmd->args[i];
        char *plus = ft_strchr(arg, '+');
        if (plus && plus[1] == '=')
        {
            if (handle_export_append(arg, env))
                status = 1;
        }
        else if (handle_export_token(arg, env))
            status = 1;
    }
    return status;
}

