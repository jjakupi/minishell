#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Parse the cd command.
// Expects first token to be "cd" and exactly one argument after it.
t_command *parse_cd(t_token *tokens)
{
    if (!tokens || ft_strcmp(tokens->value, "cd") != 0)
    {
        fprintf(stderr, "Error: Expected cd command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = ft_strdup("cd");

    t_token *current = tokens->next; // Move past "cd"

    // If no argument is provided, use the HOME environment variable
    if (!current)
    {
        char *home = getenv("HOME");
        if (home)
        {
            add_argument(command, home);
        }
        // Return the command; no error is printed.
        return command;
    }

    // Check if there is more than one argument.
    if (current->next)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return command;
    }

    if (current->type != WORD)
    {
        fprintf(stderr, "cd: invalid argument\n");
        return command;
    }

    add_argument(command, current->value);

    return command;
}


