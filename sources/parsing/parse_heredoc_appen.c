#include "../include/minishell.h"

int	parse_heredoc(t_command *cmd, t_token **current)
{
	if (!*current || (*current)->type != HEREDOC)
		return (-1);

	t_token *redir = *current;
	t_token *delim_token = redir->next;

	if (!delim_token)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
		return (-1);
	}

	if (delim_token->type != WORD)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `%s'\n", delim_token->value);
		return (-1);
	}

	// Optional: Reject <<- for now
	if (strcmp(redir->value, "<<-") == 0)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `<<-'\n");
		return (-1);
	}

	if (has_unmatched_quotes(delim_token->value))
	{
		// Bash stays silent here (multi-line input); you can just return 0 and keep parsing
		return (0);
	}

	cmd->heredoc_delimiter = ft_strdup(delim_token->value);
	cmd->has_heredoc = 1;

	// Detect if delimiter is quoted
	if (delim_token->value[0] == '\'' || delim_token->value[0] == '"')
		cmd->expand_heredoc = 0;
	else
		cmd->expand_heredoc = 1;

	// Advance pointer
	*current = delim_token->next;
	return (0);
}


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
