/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cd_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:29:22 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 12:19:38 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_command	*parse_cd_args(t_command *cmd, t_token *current)
{
	if (current == NULL)
	{
		add_argument(cmd, NULL);
		return (cmd);
	}
	if (current->type == PIPE
		|| current->type == REDIR_IN
		|| current->type == REDIR_OUT
		|| current->type == REDIR_APPEND
		|| current->type == HEREDOC)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		free_command(cmd);
		return (NULL);
	}
	add_argument(cmd, current->value);
	return (cmd);
}

t_command	*parse_cd(t_token *tokens)
{
	t_command	*cmd;
	t_token		*current;

	cmd = create_command();
	if (cmd == NULL)
		return (NULL);
	cmd->cmd = ft_strdup("cd");
	if (cmd->cmd == NULL)
	{
		free_command(cmd);
		return (NULL);
	}
	current = tokens->next;
	return (parse_cd_args(cmd, current));
}

static t_command	*parse_export_args(t_command *cmd, t_token *current)
{
	while (current)
	{
		if (current->type == WORD)
		{
			if (!is_valid_export_token(current->value))
			{
				fprintf(stderr,
					"export: `%s': not a valid identifier\n",
					current->value);
				free_command(cmd);
				return (NULL);
			}
			add_argument(cmd, current->value);
		}
		current = current->next;
	}
	return (cmd);
}

t_command	*parse_export(t_token *tokens)
{
	t_command	*cmd;
	t_token		*current;

	if (tokens == NULL || ft_strcmp(tokens->value, "export") != 0)
	{
		fprintf(stderr, "Error: Expected export command\n");
		return (NULL);
	}
	cmd = create_command();
	if (cmd == NULL)
		return (NULL);
	cmd->cmd = ft_strdup("export");
	if (cmd->cmd == NULL)
	{
		free_command(cmd);
		return (NULL);
	}
	current = tokens->next;
	return (parse_export_args(cmd, current));
}
/*
t_command *parse_cd(t_token *tokens)
{
	if (!tokens || ft_strcmp(tokens->value, "cd") != 0)
	{
		printf("minishell: Error: Expected cd command\n");
		return NULL;
	}

	t_command *command = create_command();
	if (!command)
		return NULL;

	command->cmd = ft_strdup("cd");
	if (!command->cmd)
	{
		free_command(command);
		return NULL;
	}

	t_token *current = tokens->next;
	if (!current)
	{
		add_argument(command, NULL); // explicitly handle no argument
		return command;
	}
	// Explicit syntax check for redirection or pipe tokens as first argument
	if (current->type == PIPE || current->type == REDIR_IN ||
		current->type == REDIR_OUT || current->type == REDIR_APPEND ||
		current->type == HEREDOC)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		free_command(command);
		return NULL;
	}
	add_argument(command, current->value);

	return command;
}


t_command *parse_export(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "export") != 0)
    {
        fprintf(stderr, "Error: Expected export command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = strdup("export");
	if (!command->cmd)
	{
		free_command(command);
		return NULL;
	}
    t_token *current = tokens->next; // Skip the "export" token

    while (current)
    {
        if (current->type == WORD)
        {
            // Validate the token (e.g. "VAR=HELLO")
            if (!is_valid_export_token(current->value))
            {
                fprintf(stderr, "export: `%s': not a valid identifier\n",
					current->value);
                free_command(command);
                return NULL;
            }
            add_argument(command, current->value);
        }
        current = current->next;
    }
    return command;
}
*/
