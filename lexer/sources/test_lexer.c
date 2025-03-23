#include "../includes/lexer.h"

// Assume free_command_list(t_command *head) is implemented
// and your other parser functions (parse_echo, parse_cd, etc.) are defined.

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

    printf("Minishell: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    // Remove trailing newline, if any.
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
        input[len - 1] = '\0';

    // Lexing phase: tokenize the input.
    t_token *tokens = tokenize(input);
    if (!tokens)
        return 1;
    print_tokens(tokens);

    // Parsing phase: parse the pipeline into a linked list of t_command
    t_command *commands = parse_pipeline(tokens);
    if (!commands) {
        fprintf(stderr, "Parsing failed.\n");
        free_tokens(tokens);
        return 1;
    }

    // Print the parsed pipeline
    t_command *cur = commands;
    int cmd_num = 1;
    while (cur)
    {
        printf("\n--- Command %d ---\n", cmd_num++);
        if (cur->cmd)
            printf("Command: %s\n", cur->cmd);
        if (cur->input_file)
            printf("Input Redirection: %s\n", cur->input_file);
        if (cur->output_file)
            printf("Output Redirection: %s (%s mode)\n",
                   cur->output_file, cur->append_mode ? "append" : "overwrite");
        if (cur->has_heredoc)
            printf("Heredoc: %s (%s)\n",
                   cur->heredoc_delimiter, cur->expand_heredoc ? "expand" : "no expand");
        printf("Arguments count: %d\n", cur->arg_count);
        for (int i = 0; i < cur->arg_count; i++)
            printf("  arg[%d]: %s\n", i, cur->args[i]);

        cur = cur->next;
    }

    // Cleanup: free the token list and command list.
    free_tokens(tokens);
    free_command(commands);
    return 0;
}

