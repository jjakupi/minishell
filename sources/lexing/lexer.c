/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:28:08 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/16 11:57:15 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	process_token(const char *input, int *i, t_token **tokens,
		char **current_arg)
{
	if (is_whitespace(input[*i]))
		process_whitespace(i, tokens, current_arg);
	else if (is_special(input[*i]))
		process_special(input, i, tokens, current_arg);
	else if (input[*i] == '\'' || input[*i] == '"')
	{
		if (process_quotes(input, i, current_arg))
			return (-1);
	}
	else
		*current_arg = append_char(*current_arg, input[(*i)++]);
	return (0);
}

t_token	*tokenize(const char *input)
{
	int		i;
	t_token	*tokens;
	char	*current_arg;

	i = 0;
	tokens = NULL;
	current_arg = NULL;
	while (input[i])
	{
		if (process_token(input, &i, &tokens, &current_arg) < 0)
		{
			free(current_arg);
			free_tokens(tokens);
			return (NULL);
		}
	}
	flush_current_arg(&tokens, &current_arg);
	return (tokens);
}
