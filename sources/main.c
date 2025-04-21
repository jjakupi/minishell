// main.c
#include "../include/minishell.h"

// main.c
#include "../include/minishell.h"

int main(void)
{
    char       *input;
    t_token    *tokens;
    t_command  *cmds;
    int         parse_status;
    int         last_exit_status = 0;

    while (1)
    {
        input = readline(PROMPT);
        if (!input) { printf("exit\n"); break; }
        if (*input) add_history(input);

        tokens = tokenize(input);
        if (!tokens) { free(input); continue; }

        parse_status = parse_pipeline(tokens, &cmds);
        free_tokens(tokens);
        if (parse_status == 2 || !cmds)
        {
            free(input);
            last_exit_status = 2;
            continue;
        }

        // Expand vars now that parsing is done
        for (t_command *c = cmds; c; c = c->next)
            expand_command_arguments(c, last_exit_status);

        // Actually run it (no more debug‐prints)
        last_exit_status = execute_command(cmds);

        free_command(cmds);
        free(input);
    }
    return last_exit_status;
}

