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

int parse_heredoc(t_command *cmd, t_token **current)
{
    char *raw;
    // 1) Get the token after '<<'
    if (check_next_token(*current, &raw) == -1)
        return -1;

    // 2) Strip any surrounding quotes so  '"hi"'  or  '\'hi\''
    //    becomes  "hi"
    char *clean = remove_surrounding_quotes(raw);
    if (!clean)
        return -1;  // strdup/malloc failure

    // 3) If the raw token had unmatched quotes, mimic bash’s
    //    “silent multiline” behavior and just keep going
    if (has_unmatched_quotes(raw))
    {
        free(clean);
        return 0;
    }

    // 4) Store the cleaned delimiter
    if (set_redirection_file(&cmd->heredoc_delimiter, clean) == -1)
    {
        free(clean);
        return -1;
    }
    free(clean);

    cmd->has_heredoc    = 1;
    // only do expansion if the user did *not* quote the delimiter
    cmd->expand_heredoc = (raw[0] != '\'' && raw[0] != '\"');

    // 5) advance past the '<<' and its argument
    *current = (*current)->next->next;
    return 0;
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

