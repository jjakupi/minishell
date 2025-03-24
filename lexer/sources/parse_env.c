#include "../includes/minishell.h"

t_command *parse_env(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "env") != 0) {
        fprintf(stderr, "Error: Expected env command\n");
        return NULL;
    }

    // Create the command structure.
    t_command *command = create_command();
    command->cmd = strdup("env");

    // Check if there are extra tokens.
    if (tokens->next) {
        fprintf(stderr, "env: too many arguments\n");
        free_command(command);
        return NULL;
    }

    return command;
}
