#include "../includes/lexer.h"

// Existing function from your lexing tester
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

    // Parsing phase: use your existing command parsers.
    t_command *cmd = NULL;
    if (tokens && tokens->value)
    {
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
        {
            // If the first token doesn't match a known command,
            // create a generic command (this helps for redirection-only lines)
            cmd = create_command();
            if (!cmd) {
                fprintf(stderr, "Failed to create command structure.\n");
                free_tokens(tokens);
                return 1;
            }
            cmd->cmd = ft_strdup(tokens->value);
            if (!cmd->cmd) {
                perror("ft_strdup");
                free_tokens(tokens);
                free_command(cmd);
                return 1;
            }
        }
    }
    else
    {
        fprintf(stderr, "No tokens found.\n");
        free_tokens(tokens);
        return 1;
    }

    // Additional parsing: iterate over tokens and process redirection tokens.
    // This loop now checks for both input ('<') and output ('>') redirection.
    t_token *current = tokens;
    while (current)
    {
        if (current->type == REDIR_IN)
        {
            // Process input redirection.
            if (parse_input_redirection(cmd, &current) == -1)
            {
                free_tokens(tokens);
                free_command(cmd);
                return 1;
            }
            continue; // current is advanced inside parse_input_redirection.
        }
        else if (current->type == REDIR_OUT)
        {
            // Process output redirection.
            if (parse_output_redirection(cmd, &current) == -1)
            {
                free_tokens(tokens);
                free_command(cmd);
                return 1;
            }
            continue; // current is advanced inside parse_output_redirection.
        }
        // Process other tokens (e.g., WORD tokens for command and arguments)
        current = current->next;
    }

    // Print the parsed command structure.
    if (cmd)
    {
        printf("\n--- Parsed Command Structure ---\n");
        if (cmd->cmd)
            printf("Command: %s\n", cmd->cmd);
        if (cmd->input_file)
            printf("Input Redirection: %s\n", cmd->input_file);
        if (cmd->output_file)
            printf("Output Redirection: %s (%s mode)\n",
                   cmd->output_file,
                   cmd->append_mode ? "append" : "overwrite");
        printf("Arguments count: %d\n", cmd->arg_count);
        for (int i = 0; i < cmd->arg_count; i++)
            printf("  arg[%d]: %s\n", i, cmd->args[i]);
    }
    else
    {
        fprintf(stderr, "Parsing failed or unknown command.\n");
    }

    // Cleanup: free the token list and command structure.
    free_tokens(tokens);
    free_command(cmd);
    return 0;
}

