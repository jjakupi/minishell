#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// parse_export: Expects the first token to be "export".
// It processes extra tokens by checking if they are valid export arguments.
// If any extra token is invalid, it prints an error and returns NULL.
t_command *parse_export(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "export") != 0)
    {
        fprintf(stderr, "Error: Expected export command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = strdup("export");

    t_token *current = tokens->next; // Skip the "export" token

    while (current)
    {
        if (current->type == WORD)
        {
            // Validate the token (e.g. "VAR=HELLO")
            if (!is_valid_export_token(current->value))
            {
                fprintf(stderr, "export: `%s': not a valid identifier\n", current->value);
                free_command(command);
                return NULL;
            }
            // If valid, add it to the command's arguments (or ignore it if you prefer).
            add_argument(command, current->value);
        }
        current = current->next;
    }
    return command;
}
