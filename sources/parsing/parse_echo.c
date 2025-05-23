/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:34:46 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 17:00:24 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_echo_word(t_command *cmd, t_token **tok)
{
	if ((*tok)->type == WORD || (*tok)->type == ENV_VAR)
	{
		add_argument(cmd, (*tok)->value);
		*tok = (*tok)->next;
		return (1);
	}
	return (0);
}

static int	echo_redir_syntax(t_command *cmd, t_token *cur)
{
	if (cur->next == NULL || cur->next->type != WORD)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		free_command(cmd);
		return (-1);
	}
	return (0);
}

static int	echo_do_redirection(t_command *cmd, t_token **tok)
{
	t_token	*cur;

	cur = *tok;
	if (cur->type == REDIR_OUT)
		return (parse_output_redirection(cmd, tok));
	if (cur->type == REDIR_APPEND)
		return (parse_append_redirection(cmd, tok));
	if (cur->type == REDIR_IN)
		return (parse_input_redirection(cmd, tok));
	if (cur->type == HEREDOC)
		return (parse_heredoc(cmd, tok));
	return (0);
}

static int	handle_echo_redir(t_command *cmd, t_token **tok)
{
	t_token	*cur;
	int		status;

	cur = *tok;
	if (!is_redirection(cur->type))
		return (0);
	status = echo_redir_syntax(cmd, cur);
	if (status < 0)
		return (-1);
	status = echo_do_redirection(cmd, tok);
	if (status < 0)
	{
		free_command(cmd);
		return (-1);
	}
	return (1);
}

t_command	*parse_echo(t_token *tokens)
{
	t_command	*cmd;
	t_token		*cur;
	int			ret;

	cmd = init_echo_command();
	if (cmd == NULL)
		return (NULL);
	cur = NULL;
	if (tokens)
		cur = tokens->next;
	while (cur)
	{
		if (handle_echo_word(cmd, &cur))
			continue ;
		ret = handle_echo_redir(cmd, &cur);
		if (ret < 0)
			return (NULL);
		if (ret)
			continue ;
		if (handle_echo_pipe(cmd, &cur) < 0)
			return (NULL);
		cur = cur->next;
	}
	return (cmd);
}
