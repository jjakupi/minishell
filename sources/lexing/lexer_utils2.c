/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:07:58 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/25 16:27:47 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	process_whitespace(int *i, t_token **tokens, char **current_arg)
{
	flush_current_arg(tokens, current_arg);
	(*i)++;
}

void	flush_current_arg(t_token **tokens, char **current_arg)
{
	if (*current_arg)
	{
		add_token(tokens, new_token(WORD, *current_arg));
		free(*current_arg);
		*current_arg = NULL;
	}
}
