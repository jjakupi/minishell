#include "../include/minishell.h"

int parse_pipeline(t_token *tokens, t_command **result)
{
	t_command *head = NULL;
	t_command *current_cmd = NULL;
	t_token *start = tokens;
	t_token *current = tokens;

	// Explicit initial check for operators at the start
	if (tokens && (tokens->type == PIPE || tokens->type == REDIR_IN ||
	               tokens->type == REDIR_OUT || tokens->type == REDIR_APPEND ||
	               tokens->type == HEREDOC))
	{
		if (!tokens->next)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			*result = NULL;
			return 2;
		}
		else if (tokens->next->type == PIPE || tokens->next->type == REDIR_IN ||
		         tokens->next->type == REDIR_OUT || tokens->next->type == REDIR_APPEND ||
		         tokens->next->type == HEREDOC)
		{
			printf("minishell: syntax error near unexpected token `%s'\n", tokens->next->value);
			*result = NULL;
			return 2;
		}
	}

	while (current)
	{
		if (current->type == PIPE)
		{
			if (!start || start->type == PIPE || !current->next)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->next ? "|" : "newline");
				free_command(head);
				*result = NULL;
				return 2;
			}

			t_token *next = current->next;
			current->next = NULL;

			t_command *parsed = parse_single_command(start);
			if (!parsed)
			{
				free_command(head);
				current->next = next;
				*result = NULL;
				return 2;
			}

			if (!head)
				head = parsed;
			else
				current_cmd->next = parsed;
			current_cmd = parsed;

			current->next = next;
			start = next;
			current = next;
			continue;
		}
		current = current->next;
	}

	if (start)
	{
		if (start->type == PIPE)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			free_command(head);
			*result = NULL;
			return 2;
		}
		t_command *parsed = parse_single_command(start);
		if (!parsed)
		{
			free_command(head);
			*result = NULL;
			return 2;
		}
		if (!head)
			head = parsed;
		else
			current_cmd->next = parsed;
	}

	*result = head;
	return 0;
}

