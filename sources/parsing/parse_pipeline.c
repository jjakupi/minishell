/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:02:12 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 17:00:36 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token	*find_prev_pipe(t_token *start, t_token *pipe_tok)
{
	t_token	*prev;

	prev = start;
	while (prev->next && prev->next != pipe_tok)
		prev = prev->next;
	return (prev);
}

static int	append_segment(t_token *seg, t_command **head, t_command **tail)
{
	t_command	*cmd;

	cmd = parse_single_command(seg);
	if (cmd == NULL)
		return (1);
	if (*head == NULL)
		*head = cmd;
	else
		(*tail)->next = cmd;
	*tail = cmd;
	return (0);
}

static int	handle_pipe_split(t_token **startp, t_token **curp,
	t_command **head, t_command **tail)
{
	t_token	*start;
	t_token	*cur;
	t_token	*prev;

	start = *startp;
	cur = *curp;
	prev = find_prev_pipe(start, cur);
	if (start == cur)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_command(*head);
		return (2);
	}
	prev->next = NULL;
	if (append_segment(start, head, tail))
	{
		free_command(*head);
		return (2);
	}
	prev->next = cur;
	*startp = cur->next;
	*curp = *startp;
	return (0);
}

static int	process_all_segments(t_token *tokens, t_command **head,
	t_command **tail)
{
	t_token	*start;
	t_token	*cur;
	int		ret;

	start = tokens;
	cur = tokens;
	while (cur)
	{
		if (cur->type == PIPE)
		{
			ret = handle_pipe_split(&start, &cur, head, tail);
			if (ret)
				return (ret);
			continue ;
		}
		cur = cur->next;
	}
	if (start && append_segment(start, head, tail))
	{
		free_command(*head);
		return (2);
	}
	return (0);
}

int	parse_pipeline(t_token *tokens, t_command **result)
{
	t_command	*head;
	t_command	*tail;
	int			code;

	if (check_syntax_errors(tokens))
	{
		*result = NULL;
		return (2);
	}
	head = NULL;
	tail = NULL;
	code = process_all_segments(tokens, &head, &tail);
	if (code)
	{
		*result = NULL;
		return (code);
	}
	*result = head;
	return (0);
}
