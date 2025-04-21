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
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);

        tokens = tokenize(input);
        if (!tokens) { free(input); continue; }

        parse_status = parse_pipeline(tokens, &cmds);
        if (parse_status == 2 || !cmds)
        {
            free_tokens(tokens);
            free(input);
            last_exit_status = 2;
            continue;
        }

        // ** NEW ** perform expansions now that we know each token’s quote
        for (t_command *c = cmds; c; c = c->next)
            expand_command_arguments(c, last_exit_status);

        // Debug print out your fully expanded commands
        for (t_command *c = cmds; c; c = c->next)
        {
            printf("[COMMAND] Name: '%s'\n", c->cmd);
            for (int x = 0; x < c->arg_count; x++)
                printf("  [ARG %d]: '%s'\n", x, c->args[x]);
            if (c->input_file)
                printf("  [INPUT FILE]: '%s'\n", c->input_file);
            if (c->output_file)
                printf("  [OUTPUT  ]: '%s' (append:%d)\n",
                       c->output_file, c->append_mode);
            if (c->has_heredoc)
                printf("  [HEREDOC  ]: '%s'\n", c->heredoc_delimiter);
        }

        // Here you’d normally dispatch to execute_builtin() or your fork/exec logic
        if (cmds)
            last_exit_status = execute_command(cmds);
        free_command(cmds);
        free_tokens(tokens);
        free(input);
    }
    return 0;
}

