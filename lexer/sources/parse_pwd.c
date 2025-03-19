#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
