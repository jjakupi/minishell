#include "../include/minishell.h"

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
int main(void)
{
    int last_exit_status = 0;

    while (1)
    {
        char *input = readline(PROMPT);
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // Add to history if input is not empty
        if (*input)
            add_history(input);

        // 🔧 Trigger rl_clear_history for test
        if (strcmp(input, "history -c") == 0)
        {
            rl_clear_history();
            free(input);
            continue;
        }

        // Tokenize input
        t_token *tokens = tokenize(input);
        if (!tokens)
        {
            // A syntax error occurred during tokenization (e.g. unmatched quote)
            last_exit_status = 2;
            free(input);
            continue;
        }

        // Build command pipeline
        t_command *commands = parse_pipeline(tokens);
        if (!commands)
        {
            // A syntax error occurred during parsing
            last_exit_status = 2;
            free_tokens(tokens);
            free(input);
            continue;
        }

        // Expand command arguments
        t_command *cur_cmd = commands;
        while (cur_cmd)
        {
            expand_command_arguments(cur_cmd, last_exit_status);
            cur_cmd = cur_cmd->next;
        }

        // Execute built-in commands
        cur_cmd = commands;
        while (cur_cmd)
        {
            int status = execute_builtin(cur_cmd);
            if (status == -1)
            {
                fprintf(stderr, "%s: command not found\n", cur_cmd->cmd);
                status = 127;
            }
            last_exit_status = status;
            cur_cmd = cur_cmd->next;
        }

        free_command(commands);
        free_tokens(tokens);
        free(input);
    }
    return 0;
}
