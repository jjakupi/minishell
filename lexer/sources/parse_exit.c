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
    if (!command)
        return NULL;
    command->cmd = strdup("exit");
    if (!command->cmd) {
        free_command(command);
        return NULL;
    }

    // Do not process any arguments after "exit"
    // This mimics your terminal's behavior where extra tokens are ignored.

    return command;
}
