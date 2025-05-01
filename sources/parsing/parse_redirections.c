/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:47:14 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 15:02:27 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parse_input_redirection(t_command *cmd, t_token **cur)
{
	char	*raw;

	if (check_next_token(*cur, &raw) < 0)
		return (-1);
	if (push_input(cmd, raw) < 0)
		return (-1);
	*cur = (*cur)->next->next;
	return (0);
}

int	parse_output_redirection(t_command *cmd, t_token **cur)
{
	char	*raw;

	if (check_next_token(*cur, &raw) < 0)
		return (-1);
	if (push_output(cmd, raw, 0) < 0)
		return (-1);
	*cur = (*cur)->next->next;
	return (0);
}

int	parse_append_redirection(t_command *cmd, t_token **cur)
{
	char	*raw;

	if (check_next_token(*cur, &raw) < 0)
		return (-1);
	if (push_output(cmd, raw, 1) < 0)
		return (-1);
	*cur = (*cur)->next->next;
	return (0);
}

int	parse_heredoc(t_command *cmd, t_token **cur)
{
	char	*raw;

	if (check_next_token(*cur, &raw) < 0)
		return (-1);
	cmd->has_heredoc = 1;
	cmd->heredoc_delimiter = remove_surrounding_quotes(raw);
	*cur = (*cur)->next->next;
	return (0);
}

int	parse_redirections(t_command *cmd, t_token **tokens)
{
	if (!*tokens)
		return (-1);
	if ((*tokens)->type == REDIR_IN)
		return (parse_input_redirection(cmd, tokens));
	if ((*tokens)->type == REDIR_OUT)
		return (parse_output_redirection(cmd, tokens));
	if ((*tokens)->type == REDIR_APPEND)
		return (parse_append_redirection(cmd, tokens));
	if ((*tokens)->type == HEREDOC)
		return (parse_heredoc(cmd, tokens));
	return (0);
}
