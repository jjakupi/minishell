#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    cmd->input_file = ft_strdup(filename_token->value);
    if (!cmd->input_file)
    {
        perror("ft_strdup");
        return -1;
    }

    // Advance the token pointer past the redirection operator and its filename.
    *current = filename_token->next;
    return 0;
}
