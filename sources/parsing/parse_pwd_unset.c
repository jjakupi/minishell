#include "../include/minishell.h"

// Assuming t_command and create_command are defined in your parsing.h
t_command *parse_pwd(t_token *tokens)
{
    if (!tokens || ft_strcmp(tokens->value, "pwd") != 0)
    {
        fprintf(stderr, "Error: Expected pwd command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = ft_strdup("pwd");
    // Ignore any extra tokens: free them or just skip over them.
    // Here we simply skip them.
    // For a complete solution, you might want to free them if needed.

    return command;
}

t_command *parse_unset(t_token *tokens)
{
    if (!tokens || ft_strcmp(tokens->value, "unset") != 0)
    {
        fprintf(stderr, "Error: Expected unset command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = ft_strdup("unset");

    t_token *current = tokens->next; // Skip the "unset" token.

    // Process each extra token.
    while (current)
    {
        if (current->type == WORD)
        {
            // Validate the token as a proper identifier.
            if (!is_valid_identifier(current->value))
            {
                // For invalid identifiers, Bash prints an error message.
                fprintf(stderr, "unset: `%s': not a valid identifier\n", current->value);
                free_command(command);
                return NULL;
            }
            // If valid, add it as an argument.
            add_argument(command, current->value);
        }
        current = current->next;
    }

    return command;
}
