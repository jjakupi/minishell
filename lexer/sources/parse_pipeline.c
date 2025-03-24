#include "../includes/minishell.h"

t_command *parse_pipeline(t_token *tokens)
{
	t_command *head = NULL;
	t_command *current_cmd = NULL;

	t_token *start = tokens;
	t_token *current = tokens;

	if (tokens && tokens->type == PIPE) {
		fprintf(stderr, "minishell: syntax error near unexpected token `|\n");
		return NULL;
	}

	while (current)
	{
		if (current->type == PIPE)
		{
			if (!start || start->type == PIPE || !current->next)
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n",
						current->next ? "|" : "newline");
				free_command(head);
				return NULL;
			}

			t_token *next = current->next;
			current->next = NULL;

			t_command *parsed = parse_single_command(start);
			if (!parsed)
			{
				free_command(head);
				current->next = next;
				return NULL;
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
		if (start->type == PIPE) {
			fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
			free_command(head);
			return NULL;
		}
		t_command *parsed = parse_single_command(start);
		if (!parsed)
		{
			free_command(head);
			return NULL;
		}
		if (!head)
			head = parsed;
		else
			current_cmd->next = parsed;
	}

	return head;
}
