#include "../includes/lexer.h"
// Existing function from your lexing tester
/**/
void print_tokens(t_token *head)
{
    t_token *current = head;
    if (!current)
    {
        printf("DEBUG: No tokens to print!\n");
        return;
    }
    while (current)
    {
        printf("Token type: %s, value: '%s'\n",
            token_type_to_str(current->type),
            current->value ? current->value : "(null)");
        current = current->next;
    }
}

int main(void) {
    char input[1024];

    printf("Enter command: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    // Remove trailing newline, if any.
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // Lexing phase: tokenize the input.
    t_token *tokens = tokenize(input);
    if (!tokens) {
        fprintf(stderr, "Tokenization failed.\n");
        return 1;
    }
    print_tokens(tokens);

    // Parsing phase: decide which parser to use based on the first token.
    t_command *cmd = NULL;
    if (strcmp(tokens->value, "echo") == 0)
        cmd = parse_echo(tokens);
    else if (strcmp(tokens->value, "cd") == 0)
        cmd = parse_cd(tokens);
    else if (strcmp(tokens->value, "pwd") == 0)
        cmd = parse_pwd(tokens);
    else if (strcmp(tokens->value, "export") == 0)
        cmd = parse_export(tokens);
    else if (strcmp(tokens->value, "unset") == 0)
        cmd = parse_unset(tokens);
    else if (strcmp(tokens->value, "env") == 0)
        cmd = parse_env(tokens);
    else if (strcmp(tokens->value, "exit") == 0)
        cmd = parse_exit(tokens);
    else
        fprintf(stderr, "Unknown command: %s\n", tokens->value);

    // Print the parsed command structure.
    if (cmd) {
        printf("Parsed Command: %s\n", cmd->cmd);
        printf("Arguments (%d):\n", cmd->arg_count);
        for (int i = 0; i < cmd->arg_count; i++) {
            printf("  arg[%d]: %s\n", i, cmd->args[i]);
        }
    } else {
        fprintf(stderr, "Parsing failed or unknown command.\n");
    }

    // Cleanup: free the token list and command structure.
    free_tokens(tokens);
    if (cmd)
        free_command(cmd);
    return 0;
}
