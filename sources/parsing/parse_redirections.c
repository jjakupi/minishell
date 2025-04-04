#include "../include/minishell.h"

// Helper function to set file values safely
int set_redirection_file(char **dest, char *src)
{
	if (*dest)
		free(*dest);
	*dest = ft_strdup(src);
	if (!(*dest))
	{
		perror("strdup");
		return (-1);
	}
	return (0);
}

// Parsing input redirection '<'
int parse_input_redirection(t_command *cmd, t_token **current)
{
	char *value;

	if (check_next_token(*current, &value) == -1)
		return (-1);
	if (set_redirection_file(&cmd->input_file, value) == -1)
		return (-1);
	*current = (*current)->next->next;
	return (0);
}

// Parsing output redirection '>'
int parse_output_redirection(t_command *cmd, t_token **current)
{
	char *value;

	if (check_next_token(*current, &value) == -1)
		return (-1);
	if (set_redirection_file(&cmd->output_file, value) == -1)
		return (-1);
	cmd->append_mode = 0;
	*current = (*current)->next->next;
	return (0);
}

// Parsing append redirection '>>'
int parse_append_redirection(t_command *cmd, t_token **current)
{
	char *value;

	if (check_next_token(*current, &value) == -1)
		return (-1);
	if (set_redirection_file(&cmd->output_file, value) == -1)
		return (-1);
	cmd->append_mode = 1;
	*current = (*current)->next->next;
	return (0);
}

// Parsing heredoc redirection '<<'
int parse_heredoc(t_command *cmd, t_token **current)
{
	char *value;

	if (check_next_token(*current, &value) == -1)
		return (-1);
	if (has_unmatched_quotes(value))
		return (0);  // Bash-like silent behavior
	if (set_redirection_file(&cmd->heredoc_delimiter, value) == -1)
		return (-1);
	cmd->has_heredoc = 1;
	cmd->expand_heredoc = (value[0] != '\'' && value[0] != '"');
	*current = (*current)->next->next;
	return (0);
}

int parse_redirections(t_command *cmd, t_token **tokens)
{
    if (!*tokens)
        return -1;

    if ((*tokens)->type == REDIR_OUT)
        return parse_output_redirection(cmd, tokens);
    else if ((*tokens)->type == REDIR_APPEND)
        return parse_append_redirection(cmd, tokens);
    else if ((*tokens)->type == REDIR_IN)
        return parse_input_redirection(cmd, tokens);
    else if ((*tokens)->type == HEREDOC)
        return parse_heredoc(cmd, tokens);

    return 0;
}

