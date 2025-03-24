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
    int last_exit_status = 0;

    printf("Minishell: ");
    if (!fgets(input, sizeof(input), stdin))
        return 1;
    input[strcspn(input, "\n")] = 0;

    t_token *tokens = tokenize(input);
    if (!tokens) return 1;

    t_command *commands = parse_pipeline(tokens);
    if (!commands) {
        free_tokens(tokens);
        return 1;
    }

    // Explicitly expand command arguments clearly (DO NOT SKIP THIS)
    t_command *cur_cmd = commands;
    while (cur_cmd) {
        expand_command_arguments(cur_cmd, last_exit_status);
        cur_cmd = cur_cmd->next;
    }

    // Print commands AFTER expansion (for testing)
    cur_cmd = commands;
    int cmd_num = 1;
    while (cur_cmd)
    {
        printf("\n--- Command %d ---\n", cmd_num++);
        if (cur_cmd->cmd)
            printf("Command: %s\n", cur_cmd->cmd);
        if (cur_cmd->input_file)
            printf("Input Redirection: %s\n", cur_cmd->input_file);
        if (cur_cmd->output_file)
            printf("Output Redirection: %s (%s mode)\n",
                   cur_cmd->output_file, cur_cmd->append_mode ? "append" : "overwrite");
        if (cur_cmd->has_heredoc)
            printf("Heredoc: %s (%s)\n",
                   cur_cmd->heredoc_delimiter, cur_cmd->expand_heredoc ? "expand" : "no expand");
        printf("Arguments count: %d\n", cur_cmd->arg_count);
        for (int i = 0; i < cur_cmd->arg_count; i++)
            printf("  arg[%d]: %s\n", i, cur_cmd->args[i]);

        cur_cmd = cur_cmd->next;
    }

    free_command(commands);
    free_tokens(tokens);
    return 0;
}
