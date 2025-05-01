#include "../include/minishell.h"

/*
 * Helper: push a filename onto the input‐files array.
 */
static int push_input(t_command *cmd, const char *raw)
{
    char *clean = remove_surrounding_quotes(raw);
    if (!clean) return -1;
    char **tmp = realloc(cmd->in_files, sizeof *tmp * (cmd->in_count + 1));
    if (!tmp) { perror("realloc"); free(clean); return -1; }
    cmd->in_files = tmp;
    cmd->in_files[cmd->in_count++] = clean;
    return 0;
}

static int push_output(t_command *cmd, const char *raw, int append)
{
    char *clean = remove_surrounding_quotes(raw);
    if (!clean) return -1;
    char **of_tmp = realloc(cmd->out_files, sizeof *of_tmp * (cmd->out_count + 1));
    int   *af_tmp = realloc(cmd->append_flags, sizeof *af_tmp * (cmd->out_count + 1));
    if (!of_tmp || !af_tmp) {
        perror("realloc");
        free(clean);
        return -1;
    }
    cmd->out_files    = of_tmp;
    cmd->append_flags = af_tmp;
    cmd->out_files[ cmd->out_count ]    = clean;
    cmd->append_flags[cmd->out_count++] = append;
    return 0;
}

/* Parser functions */

int parse_input_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) < 0) return -1;
    if (push_input(cmd, raw) < 0) return -1;
    *cur = (*cur)->next->next;
    return 0;
}

// example: parse_output_redirection
int parse_output_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) < 0) return -1;
    if (push_output(cmd, raw, /*append=*/0) < 0) return -1;
    *cur = (*cur)->next->next;
    return 0;
}


// e.g. parse_append_redirection
int parse_append_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) < 0) return -1;
    // NO open() here!  Just record it:
    if (push_output(cmd, raw, /*append=*/1) < 0) return -1;
    *cur = (*cur)->next->next;
    return 0;
}

int parse_heredoc(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) < 0) return -1;
    cmd->has_heredoc       = 1;
    cmd->heredoc_delimiter = remove_surrounding_quotes(raw);
    *cur = (*cur)->next->next;
    return 0;
}

int parse_redirections(t_command *cmd, t_token **tokens)
{
    if (!*tokens) return -1;

    if ((*tokens)->type == REDIR_IN)
        return parse_input_redirection(cmd, tokens);
    if ((*tokens)->type == REDIR_OUT)
        return parse_output_redirection(cmd, tokens);
    if ((*tokens)->type == REDIR_APPEND)
        return parse_append_redirection(cmd, tokens);
    if ((*tokens)->type == HEREDOC)
        return parse_heredoc(cmd, tokens);

    return 0;
}

