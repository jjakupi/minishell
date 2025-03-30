#include "../include/minishell.h"

t_token	*new_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("malloc failed for token");
		exit(EXIT_FAILURE);
	}
	token->type = type;
	if (value)
	{
		token->value = strdup(value);
		if (!token->value)
		{
			free(token);
			perror("strdup failed for token value");
			exit(EXIT_FAILURE);
		}
	}
	else
		token->value = NULL;
	token->next = NULL;
	return token;
}

// Efficiently add a token to the end of the linked list
void	add_token(t_token **head, t_token *new_tok)
{
	static t_token	*last = NULL;

	if (!head || !new_tok)
		return ;
	if (!*head)
	{
		*head = new_tok;
		last = new_tok;
	}
	else
	{
		last->next = new_tok;
		last = new_tok;
	}
}

// Safely free all tokens in the linked list
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

// Get a readable string representation of a token type
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
