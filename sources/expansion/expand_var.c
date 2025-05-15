/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 16:57:44 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(char **envp, const char *key)
{
	size_t	keylen;
	int		i;

	keylen = ft_strlen(key);
	if (!envp || !key)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, keylen) == 0
			&& envp[i][keylen] == '=')
			return (envp[i] + keylen + 1);
		i++;
	}
	return (NULL);
}

char	*expand_argument(const char *arg, int last_exit_status, char **envp)
{
	char	buf[4096];
	int		len;

	len = ft_strlen(arg);
	if (is_entirely_single_quoted(arg, len))
		return (handle_single_quotes(arg, len));
	process_expansion(arg, last_exit_status, buf, envp);
	return (ft_strdup(buf));
}

static void	expand_list(char **arr, int count,
	int last_status, char **envp)
{
	int		i;
	char	*exp;

	i = 0;
	while (i < count)
	{
		exp = expand_argument(arr[i], last_status, envp);
		free(arr[i]);
		arr[i] = exp;
		i++;
	}
}

void	expand_command_arguments(t_command *cmd, int last_exit_status,
			char **envp)
{
	char	*x;

	if (!cmd)
		return ;
	if (cmd->cmd)
	{
		x = expand_argument(cmd->cmd,
				last_exit_status, envp);
		free(cmd->cmd);
		cmd->cmd = x;
	}
	expand_list(cmd->args, cmd->arg_count,
		last_exit_status, envp);
	expand_list(cmd->in_files, cmd->in_count,
		last_exit_status, envp);
	expand_list(cmd->out_files, cmd->out_count,
		last_exit_status, envp);
}
