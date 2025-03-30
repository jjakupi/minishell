#include "../include/minishell.h"

int parse_input_redirection(t_command *cmd, t_token **current)
{
    // Ensure there is a token following the REDIR_IN token.
    if ((*current)->next == NULL)
    {
        fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
        return -1;
    }

    // The next token should be a WORD (filename) and not an operator.
    t_token *filename_token = (*current)->next;
    if (filename_token->type != WORD)
    {
        fprintf(stderr, "bash: syntax error near unexpected token `%s'\n", filename_token->value);
        return -1;
    }

    // In the parsing phase, we simply record the filename.
    cmd->input_file = strdup(filename_token->value);
    if (!cmd->input_file)
    {
        perror("ft_strdup");
        return -1;
    }

    // Advance the token pointer past the redirection operator and its filename.
    *current = filename_token->next;
    return 0;
}

int parse_output_redirection(t_command *cmd, t_token **current)
{

    // Ensure the current token is the '>' operator.
    if (*current == NULL || (*current)->type != REDIR_OUT) {
        fprintf(stderr, "Internal error: expected output redirection operator\n");
        return -1;
    }

    // Check for a token following the '>' operator.
    if ((*current)->next == NULL) {
        fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
        return -1;
    }

    t_token *filename_token = (*current)->next;

    // The token after '>' must be a WORD (a valid filename), not another operator.
    if (filename_token->type != WORD) {
        fprintf(stderr, "bash: syntax error near unexpected token `%s'\n",
                filename_token->value);
        return -1;
    }

    // If an output file was already specified (e.g., multiple redirections),
    // free the previous one so that the last one wins.
    if (cmd->output_file != NULL) {
        free(cmd->output_file);
        cmd->output_file = NULL;
    }

    // Record the output file name.
    cmd->output_file = strdup(filename_token->value);
    if (!cmd->output_file) {
        perror("strdup");
        return -1;
    }

    // For the '>' operator, set append_mode to 0 (overwrite mode).
    cmd->append_mode = 0;

    // Advance the token pointer past the redirection operator and the filename.
    *current = filename_token->next;

    return 0;
}
