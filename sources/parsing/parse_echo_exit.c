#include "../include/minishell.h"

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
