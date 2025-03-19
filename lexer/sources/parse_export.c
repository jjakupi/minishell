#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// parse_export: Expects the first token to be "export".
// It ignores any extra tokens that are valid identifiers,
// and prints an error if any extra token is not a valid identifier.
t_command *parse_export(t_token *tokens)
{
    if (!tokens || ft_strcmp(tokens->value, "export") != 0)
    {
        fprintf(stderr, "Error: Expected export command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = ft_strdup("export");

    t_token *current = tokens->next; // Skip the "export" token

    // Process extra tokens.
    while (current)
    {
        if (current->type == WORD)
        {
            // If the extra token is not a valid identifier, report an error.
            if (!is_valid_identifier(current->value))
            {
                fprintf(stderr, "export: `%s': not a valid identifier\n", current->value);
                free_command(command);
                return NULL;
            }
            // If it is a valid identifier, we ignore it (do not add it to arguments).
        }
        current = current->next;
    }

    // The command structure is returned with no arguments.
    return command;
}
