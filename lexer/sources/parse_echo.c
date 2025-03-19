#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Parse the echo command from a list of tokens.
// Expects the first token to be "echo", then optionally "-n", then the rest as arguments.
t_command *parse_echo(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "echo") != 0)
    {
        fprintf(stderr, "Command not found\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = strdup("echo");

    t_token *current = tokens->next; // Skip the "echo" token

    // Loop to process one or more "-n" flags.
    while (current && strcmp(current->value, "-n") == 0)
    {
        command->suppress_newline = 1; // Set the flag if -n is encountered
        current = current->next;       // Move to the next token
    }

    // Collect the remaining tokens as arguments.
    while (current)
    {
        if (current->type == WORD)
        {
            add_argument(command, current->value);
        }
        current = current->next;
    }

    return command;
}

