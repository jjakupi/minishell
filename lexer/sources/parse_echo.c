#include "../includes/minishell.h"
// Parse the echo command from a list of tokens.
// Expects the first token to be "echo", then optionally "-n", then the rest as arguments.
t_command *parse_echo(t_token *tokens)
{
    t_command *cmd = create_command();
    cmd->cmd = ft_strdup("echo");
    tokens = tokens->next;

    char buffer[4096];

    // Explicitly merge tokens into arguments with spaces
    while (tokens)
    {
        buffer[0] = '\0';
        while (tokens && (tokens->type == WORD || tokens->type == ENV_VAR))
        {
            if (buffer[0] != '\0') // explicitly add spaces between tokens
                strcat(buffer, " ");
            strcat(buffer, tokens->value);
            tokens = tokens->next;
        }
        if (buffer[0])
            add_argument(cmd, buffer);
        else if (tokens) // Skip non-WORD/ENV_VAR tokens explicitly
            tokens = tokens->next;
    }
    return cmd;
}
