/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pwd_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:41:10 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 14:46:16 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command	*parse_pwd(t_token *tokens)
{
	t_command	*command;

	if (!tokens || ft_strcmp(tokens->value, "pwd") != 0)
	{
		fprintf(stderr, "Error: Expected pwd command\n");
		return (NULL);
	}
	command = create_command();
	command->cmd = ft_strdup("pwd");
	return (command);
}

static t_command	*init_unset_command(void)
{
	t_command	*cmd;

	cmd = create_command();
	if (cmd == NULL)
		return (NULL);
	cmd->cmd = ft_strdup("unset");
	if (cmd->cmd == NULL)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}

static t_command	*process_unset_args(t_command *cmd, t_token *tok)
{
	while (tok)
	{
		if (tok->type == WORD)
		{
			if (!is_valid_identifier(tok->value))
			{
				ft_putstr_fd("unset: `", 2);
				ft_putstr_fd(tok->value, 2);
				ft_putendl_fd("': not a valid identifier", 2);
				free_command(cmd);
				return (NULL);
			}
			add_argument(cmd, tok->value);
		}
		tok = tok->next;
	}
	return (cmd);
}

t_command	*parse_unset(t_token *tokens)
{
	t_command	*cmd;
	t_token		*cur;

	if (tokens == NULL || ft_strcmp(tokens->value, "unset") != 0)
	{
		ft_putstr_fd("Error: Expected unset command\n", 2);
		return (NULL);
	}
	cmd = init_unset_command();
	if (cmd == NULL)
		return (NULL);
	cur = tokens->next;
	return (process_unset_args(cmd, cur));
}
