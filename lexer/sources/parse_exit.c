#include "../includes/minishell.h"

t_command *parse_exit(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "exit") != 0) {
        fprintf(stderr, "Error: Expected exit command\n");
        return NULL;
    }

    t_command *command = create_command();
    if (!command)
        return NULL;
    command->cmd = strdup("exit");
    if (!command->cmd) {
        free_command(command);
        return NULL;
    }

    // Process tokens after "exit" and store them as arguments.
    t_token *current = tokens->next; // Skip the "exit" token
    while (current)
    {
        if (current->type == WORD) {  // Assuming WORD tokens hold the arguments
            add_argument(command, current->value);
        }
        current = current->next;
    }

    return command;
}
