/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_single_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:03:11 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 15:05:08 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_token_parsing(t_command *cmd, t_token **tokens)
{
	while (*tokens)
	{
		if ((*tokens)->type == WORD || (*tokens)->type == ENV_VAR)
		{
			add_argument(cmd, (*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else if (is_redirection((*tokens)->type))
		{
			if (parse_redirections(cmd, tokens) == -1)
			{
				free_command(cmd);
				return (-1);
			}
		}
		else if ((*tokens)->type == PIPE)
		{
			fprintf
				(stderr, "minishell: syntax error near unexpected token `|'\n");
			return (-1);
		}
		else
			*tokens = (*tokens)->next;
	}
	return (0);
}

t_command	*parse_single_command(t_token *tokens)
{
	t_command	*cmd;

	if (!tokens)
		return (NULL);
	cmd = create_command();
	if (!cmd)
		return (NULL);
	cmd->cmd = ft_strdup(tokens->value);
	if (!cmd->cmd)
	{
		free_command(cmd);
		return (NULL);
	}
	tokens = tokens->next;
	if (handle_token_parsing(cmd, &tokens) == -1)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
