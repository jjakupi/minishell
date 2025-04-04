#include "../include/minishell.h"

t_token	*new_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		exit_with_error("malloc (new_token)");
	token->type = type;
	if (value)
	{
		token->value = ft_strdup(value);
		if (!token->value)
			exit_with_error("ft_strdup (new_token)");
	}
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}


void	add_token(t_token **head, t_token *new_tok)
{
	if (!head || !new_tok)
		return ;
	if (!*head)
		*head = new_tok;
	else
	{
		t_token *tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_tok;
	}
}


void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

const char	*token_type_to_str(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_IN)
		return ("REDIR_IN");
	if (type == REDIR_OUT)
		return ("REDIR_OUT");
	if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	if (type == END)
		return ("END");
	return ("UNKNOWN");
}
