/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:52:53 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 11:58:32 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command	*parse_env(t_token *tokens)
{
	t_command	*command;

	if (!tokens || strcmp(tokens->value, "env") != 0)
	{
		fprintf(stderr, "Error: Expected env command\n");
		return (NULL);
	}
	command = create_command();
	command->cmd = strdup("env");
	return (command);
}

static t_command	*init_exit_command(void)
{
	t_command	*cmd;

	cmd = create_command();
	if (cmd == NULL)
		return (NULL);
	cmd->cmd = ft_strdup("exit");
	if (cmd->cmd == NULL)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}

static void	fill_exit_args(t_command *cmd, t_token *tok)
{
	while (tok)
	{
		if (tok->type == WORD)
			add_argument(cmd, tok->value);
		tok = tok->next;
	}
}

t_command	*parse_exit(t_token *tokens)
{
	t_command	*cmd;
	t_token		*cur;

	if (tokens == NULL || ft_strcmp(tokens->value, "exit") != 0)
	{
		ft_putstr_fd("Error: Expected exit command\n", 2);
		return (NULL);
	}
	cmd = init_exit_command();
	if (cmd == NULL)
		return (NULL);
	cur = tokens->next;
	fill_exit_args(cmd, cur);
	return (cmd);
}
