#include "../include/minishell.h"

int parse_pipeline(t_token *tokens, t_command **result)
{
    t_command *head = NULL;
    t_command *current_cmd = NULL;
    t_token *start = tokens;
    t_token *current = tokens;

    if (check_syntax_errors(tokens))
    {
        *result = NULL;
        return 2;
    }

    while (current)
    {
        if (current->type == PIPE)
        {
            t_token *next_cmd_tokens = current->next;

            // Corrected: find the token BEFORE the PIPE to disconnect cleanly.
            t_token *prev = start;
            if (prev == current)  // Special case: PIPE at the start
            {
                printf("minishell: syntax error near unexpected token `|'\n");
                free_command(head);
                *result = NULL;
                return 2;
            }

            while (prev->next && prev->next != current)
                prev = prev->next;

            prev->next = NULL;  // Correctly isolate BEFORE PIPE

            t_command *parsed = parse_single_command(start);
            if (!parsed)
            {
                free_command(head);
                *result = NULL;
                return 2;
            }

            if (!head)
                head = parsed;
            else
                current_cmd->next = parsed;

            current_cmd = parsed;

            start = next_cmd_tokens;  // continue parsing after the PIPE
            current = next_cmd_tokens;
        }
        else
            current = current->next;
    }

    if (start)
    {
        t_command *parsed = parse_single_command(start);
        if (!parsed)
        {
            free_command(head);
            *result = NULL;
            return 2;
        }
        if (!head)
            head = parsed;
        else
            current_cmd->next = parsed;
    }

    *result = head;
    return 0;
}


