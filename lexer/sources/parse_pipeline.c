#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_command *parse_pipeline(t_token *tokens)
{
    t_command *head = NULL;
    t_command *current_cmd = NULL;

    // 'start' will point to the beginning of each command segment.
    t_token *start = tokens;
    t_token *current = tokens;

    // Check for an initial pipe error.
    if (tokens && tokens->type == PIPE) {
        fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
        return NULL;
    }

    while (current)
    {
        if (current->type == PIPE)
        {
            // Before breaking, ensure there is a valid command segment.
            if (!start || start->type == PIPE)
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
                free_command(head);
                return NULL;
            }
            if (!current->next)
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
                free_command(head);
                return NULL;
            }

            // Save pointer to tokens after the PIPE.
            t_token *next = current->next;

            // Break the chain so that the tokens for the current command stop at this PIPE.
            current->next = NULL;

            // Now parse the current command segment.
            t_command *parsed = parse_single_command(start);
            if (!parsed)
            {
                free_command(head);
                return NULL;
            }
            if (!head)
                head = parsed;
            else
                current_cmd->next = parsed;

            current_cmd = parsed;

            // Move start to the next segment.
            start = next;
            current = next;
            continue;
        }
        current = current->next;
    }

    // Parse the final command segment.
    if (start)
    {
        // Check if the final segment is valid (it shouldn't be just a pipe)
        if (start->type == PIPE) {
            fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
            free_command(head);
            return NULL;
        }
        t_command *parsed = parse_single_command(start);
        if (!parsed)
        {
            free_command(head);
            return NULL;
        }
        if (!head)
            head = parsed;
        else
            current_cmd->next = parsed;
    }

    return head;
}

