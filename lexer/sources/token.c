#include "../includes/minishell.h"

t_token	*new_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_tok)
{
	t_token	*current;

	if (!head || !new_tok)
		return ;
	if (!*head)
	{
		*head = new_tok;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_tok;
}

void free_tokens(t_token *head)
{
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
    }
}


const char	*token_type_to_str(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == ENV_VAR)
		return ("ENV_VAR");
	else if (type == END)
		return ("END");
	return ("UNKNOWN");
}
