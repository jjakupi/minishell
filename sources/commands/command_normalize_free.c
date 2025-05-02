/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_normalize_free.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:20:19 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 14:01:44 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_string_array(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_command(t_command *cmd)
{
	t_command	*next;

	if (cmd == NULL)
		return ;
	free(cmd->cmd);
	free_string_array(cmd->args, cmd->arg_count);
	free_string_array(cmd->in_files, cmd->in_count);
	free_string_array(cmd->out_files, cmd->out_count);
	free(cmd->append_flags);
	free(cmd->heredoc_delimiter);
	next = cmd->next;
	free(cmd);
	free_command(next);
}

void	normalize_empty_cmd(t_command *c)
{
	int		i;
	char	*new_cmd;

	if (c == NULL || c->cmd == NULL)
		return ;
	if (c->cmd[0] == '\0' && c->arg_count > 0)
	{
		free(c->cmd);
		new_cmd = ft_strdup(c->args[0]);
		if (new_cmd == NULL)
			return ;
		c->cmd = new_cmd;
		i = 1;
		while (i < c->arg_count)
		{
			c->args[i - 1] = c->args[i];
			i++;
		}
		c->arg_count--;
		c->args[c->arg_count] = NULL;
	}
}

void	fix_empty_cmd(t_command *c)
{
	if (c->cmd[0] == '\0' && c->arg_count > 0)
	{
		char *first = c->args[0];

		free(c->cmd);
		c->cmd = ft_strdup(first);
		if (!c->cmd)
			exit_with_error("ft_strdup");
		int i = 1;
		while (i < c->arg_count)
		{
			c->args[i - 1] = c->args[i];
			i++;
		}
		c->arg_count--;
		c->args[c->arg_count] = NULL;
	}
}

