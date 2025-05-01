/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:05:58 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 15:19:51 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	has_unmatched_quotes(const char *str)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (quote == 0 && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote != 0 && str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote != 0);
}

char	*remove_surrounding_quotes(const char *str)
{
	size_t	len;
	char	*out;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '\"' && str[len - 1] == '\"')))
	{
		out = malloc(len - 1);
		if (!out)
			return (NULL);
		ft_memcpy(out, str + 1, len - 2);
		out[len - 2] = '\0';
		return (out);
	}
	return (ft_strdup(str));
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

int	check_next_token(t_token *current, char **value)
{
	if (current->next == NULL || current->next->type != WORD)
	{
		if (current->next != NULL)
		{
			fprintf(stderr,
				"bash: syntax error near unexpected token `%s'\n",
				current->next->value);
		}
		else
		{
			fprintf(stderr,
				"bash: syntax error near unexpected token `newline'\n");
		}
		return (-1);
	}
	*value = current->next->value;
	return (0);
}
