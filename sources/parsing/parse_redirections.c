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

static int store_redir(char **dest, const char *raw)
{
    // strip any leading/trailing quotes
    char *clean = remove_surrounding_quotes(raw);
    if (!clean)
        return -1;
    if (set_redirection_file(dest, clean) == -1)
    {
        free(clean);
        return -1;
    }
    free(clean);
    return 0;
}

int parse_input_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;
    if (store_redir(&cmd->input_file, raw) == -1)
        return -1;
    *cur = (*cur)->next->next;
    return 0;
}

int parse_output_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;
    if (store_redir(&cmd->output_file, raw) == -1)
        return -1;
    cmd->append_mode = 0;
    *cur = (*cur)->next->next;
    return 0;
}

int parse_append_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;
    if (store_redir(&cmd->output_file, raw) == -1)
        return -1;
    cmd->append_mode = 1;
    *cur = (*cur)->next->next;
    return 0;
}

int parse_heredoc(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;

    // strip quotes for the *delimiter* comparison
    char *clean = remove_surrounding_quotes(raw);
    if (!clean) return -1;

    // if unmatched, bash silently ignores
    if (!has_unmatched_quotes(raw))
    {
        if (set_redirection_file(&cmd->heredoc_delimiter, clean) == -1)
        {
            free(clean);
            return -1;
        }
        cmd->has_heredoc    = 1;
        cmd->expand_heredoc = (raw[0] != '\'' && raw[0] != '"');
    }
    free(clean);

    *cur = (*cur)->next->next;
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

