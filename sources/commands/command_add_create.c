/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_add_create.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:20:19 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 19:16:47 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**alloc_new_args(char **old_args, int old_count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (old_count + 2));
	if (new_args == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < old_count)
	{
		new_args[i] = old_args[i];
		i++;
	}
	return (new_args);
}

void	add_argument(t_command *cmd, const char *arg)
{
	char	**new_args;

	new_args = alloc_new_args(cmd->args, cmd->arg_count);
	free(cmd->args);
	new_args[cmd->arg_count] = ft_strdup(arg);
	if (new_args[cmd->arg_count] == NULL)
	{
		perror("ft_strdup");
		exit(EXIT_FAILURE);
	}
	cmd->arg_count++;
	new_args[cmd->arg_count] = NULL;
	cmd->args = new_args;
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof *cmd);
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->suppress_newline = 0;
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->in_files = NULL;
	cmd->in_count = 0;
	cmd->out_files = NULL;
	cmd->out_count = 0;
	cmd->append_flags = NULL;
	cmd->heredoc_delimiter = NULL;
	cmd->has_heredoc = 0;
	cmd->expand_heredoc = 0;
	cmd->heredoc_fd		= -1;
	cmd->next = NULL;
	return (cmd);
}
