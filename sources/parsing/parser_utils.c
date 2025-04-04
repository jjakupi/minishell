#include "../include/minishell.h"

int	has_unmatched_quotes(const char *str)
{
	char	quote = 0;
	for (int i = 0; str[i]; i++)
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
	}
	return (quote != 0);
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND || type == HEREDOC);
}

int check_syntax_errors(t_token *tokens)
{
	if (!tokens)
		return (0);

	if (tokens->type == PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}

	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next || tokens->next->type == PIPE)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (1);
			}
		}
		else if (is_redirection(tokens->type))
		{
			if (!tokens->next || tokens->next->type != WORD)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					tokens->next ? tokens->next->value : "newline");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}


int check_next_token(t_token *current, char **value)
{
    if (!current->next || current->next->type != WORD)
    {
        fprintf(stderr, "bash: syntax error near unexpected token `%s'\n",
                current->next ? current->next->value : "newline");
        return -1;
    }
    *value = current->next->value;
    return 0;
}
