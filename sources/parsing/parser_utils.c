/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:05:58 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 15:18:28 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_pipe_syntax(t_token *tok)
{
	if (tok->type == PIPE)
	{
		if (tok->next == NULL || tok->next->type == PIPE)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (1);
		}
	}
	return (0);
}

static int	check_redir_syntax(t_token *tok)
{
	const char	*unexpected;

	if (is_redirection(tok->type))
	{
		if (tok->next == NULL || tok->next->type != WORD)
		{
			if (tok->next != NULL)
				unexpected = tok->next->value;
			else
				unexpected = "newline";
			printf("minishell: syntax error near unexpected token `%s'\n",
				unexpected);
			return (1);
		}
	}
	return (0);
}

int	check_syntax_errors(t_token *tokens)
{
	t_token	*cur;

	if (tokens == NULL)
		return (0);
	cur = tokens;
	while (cur)
	{
		if (check_pipe_syntax(cur) || check_redir_syntax(cur))
			return (1);
		cur = cur->next;
	}
	return (0);
}
