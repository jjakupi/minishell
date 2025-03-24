#include "../includes/minishell.h"

int parse_append_redirection(t_command *cmd, t_token **current)
{
	if (!*current || ((*current)->type != REDIR_OUT && (*current)->type != REDIR_APPEND))
		return -1;

	t_token *redir = *current;
	t_token *next = redir->next;

	// Handle missing file name
	if (!next || next->type != WORD)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `%s'\n",
			next ? next->value : "newline");
		return -1;
	}

	// Always use the last redirection
	if (cmd->output_file)
	{
		free(cmd->output_file);
		cmd->output_file = NULL;
	}

	cmd->output_file = ft_strdup(next->value);
	if (!cmd->output_file)
		return -1;

	cmd->append_mode = (redir->type == REDIR_APPEND);

	*current = next->next;
	return 0;
}
