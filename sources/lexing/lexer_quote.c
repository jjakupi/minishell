/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:07:58 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/25 16:07:34 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*build_wrapped(const char *start_ptr, int len, char quote)
{
	char	*inner;
	char	*wrapped;

	inner = malloc(len + 1);
	if (inner == NULL)
		exit_with_error("malloc");
	ft_memcpy(inner, start_ptr, len);
	inner[len] = '\0';
	wrapped = malloc(len + 3);
	if (wrapped == NULL)
		exit_with_error("malloc");
	wrapped[0] = quote;
	ft_memcpy(wrapped + 1, inner, len);
	wrapped[len + 1] = quote;
	wrapped[len + 2] = '\0';
	free(inner);
	return (wrapped);
}

void	append_wrapped(char **current_arg, const char *wrapped)
{
	char	*new_arg;
	int		len_old;
	int		len_wrap;

	if (*current_arg == NULL)
	{
		*current_arg = ft_strdup(wrapped);
		if (*current_arg == NULL)
			exit_with_error("strdup");
	}
	else
	{
		len_old = ft_strlen(*current_arg);
		len_wrap = ft_strlen(wrapped);
		new_arg = malloc(len_old + len_wrap + 1);
		if (new_arg == NULL)
			exit_with_error("malloc");
		ft_memcpy(new_arg, *current_arg, len_old);
		ft_memcpy(new_arg + len_old, wrapped, len_wrap + 1);
		free(*current_arg);
		*current_arg = new_arg;
	}
}

int	process_quotes(const char *input, int *i, char **current_arg)
{
	char	*wrapped;
	char	quote;
	int		start;
	int		len;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
		return (syntax_error("unmatched quote"));
	len = *i - start;
	wrapped = build_wrapped(input + start, len, quote);
	append_wrapped(current_arg, wrapped);
	free(wrapped);
	(*i)++;
	return (0);
}
