/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 15:25:37 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_argument(const char *arg, int last_exit_status)
{
	char	buf[4096];
	int		len;

	len = ft_strlen(arg);
	if (is_entirely_single_quoted(arg, len))
		return (handle_single_quotes(arg, len));
	process_expansion(arg, last_exit_status, buf);
	return (ft_strdup(buf));
}

void	expand_command_arguments(t_command *cmd, int last_exit_status)
{
	int		i;
	char	*expanded;

	if (cmd == NULL)
		return ;
	if (cmd->cmd != NULL)
	{
		expanded = expand_argument(cmd->cmd, last_exit_status);
		free(cmd->cmd);
		cmd->cmd = expanded;
	}
	i = 0;
	while (i < cmd->arg_count)
	{
		expanded = expand_argument(cmd->args[i], last_exit_status);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
		i++;
	}
}
