#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_command *parse_exit(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "exit") != 0) {
        fprintf(stderr, "Error: Expected exit command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = strdup("exit");

    t_token *current = tokens->next;  // Skip the "exit" token.
    if (current) {
        // For now, if more than one argument, you might print an error.
        if (current->next) {
            fprintf(stderr, "exit: too many arguments\n");
            free_command(command);
            return NULL;
        }
        // If one argument exists, validate it as numeric.
        if (!is_numeric(current->value)) {
            fprintf(stderr, "exit: %s: numeric argument required\n", current->value);
            free_command(command);
            return NULL;
        }
        // If valid, add it as the argument.
        add_argument(command, current->value);
    }
    return command;
}
