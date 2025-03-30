#include "../include/minishell.h"

// Simple token printing for debugging
void print_tokens(t_token *head)
{
    t_token *current = head;
    if (!current)
    {
        printf("No tokens to print!\n");
        return;
    }
    while (current)
    {
        printf("Token type: %s | Value: '%s'\n",
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

        if (*input)
            add_history(input);

        t_token *tokens = tokenize(input);
        if (!tokens)
        {
            fprintf(stderr, "Syntax error during tokenization\n");
            free(input);
            continue;
        }

        t_command *cmd = parse_single_command(tokens);
        if (!cmd)
        {
            fprintf(stderr, "Parsing failed\n");
            free_tokens(tokens);
            free(input);
            continue;
        }

        // Explicitly expand arguments ONLY ONCE clearly
        for (int x = 0; x < cmd->arg_count; x++)
        {
            char *expanded_arg = expand_argument(cmd->args[x], last_exit_status);
            free(cmd->args[x]);
            cmd->args[x] = expanded_arg;
        }

        // Explicitly expand command ONCE, preserving quotes until AFTER expansion
        char *expanded_cmd = expand_argument(cmd->cmd, last_exit_status);
        free(cmd->cmd);

        // Only remove quotes AFTER expansion explicitly
        cmd->cmd = remove_surrounding_quotes(expanded_cmd);
        free(expanded_cmd);

        // Debug explicitly
        printf("[DEBUG] Command after expansion: '%s'\n", cmd->cmd);
        for (int x = 0; x < cmd->arg_count; x++)
            printf("[DEBUG] Arg[%d] after expansion: '%s'\n", x, cmd->args[x]);

        int status = execute_builtin(cmd);
        if (status == -1)
        {
            fprintf(stderr, "minishell: %s: command not found\n", cmd->cmd);
            status = 127;
        }

        last_exit_status = status;

        free_command(cmd);
        free_tokens(tokens);
        free(input);
    }
    return 0;
}



char *remove_surrounding_quotes(const char *str)
{
    size_t len = strlen(str);
    if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') ||
                     (str[0] == '\'' && str[len - 1] == '\'')))
        return ft_substr(str, 1, len - 2);
    return strdup(str);
}
