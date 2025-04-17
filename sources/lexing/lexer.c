#include "../include/minishell.h"

t_token *tokenize(const char *input)
{
    int      i = 0;
    t_token *tokens = NULL;
    char    *current_arg = NULL;

    while (input[i])
    {
        if (is_whitespace(input[i]))
            process_whitespace(&i, &tokens, &current_arg);
        else if (is_special(input[i]))
            process_special(input, &i, &tokens, &current_arg);
        else if (input[i] == '\'' || input[i] == '"')
        {
            // don’t flush here—just stitch into current_arg
            if (process_quotes(input, &i, &current_arg))
            {
                free(current_arg);
                free_tokens(tokens);
                return NULL;
            }
        }
        else
            current_arg = append_char(current_arg, input[i++]);
    }
    flush_current_arg(&tokens, &current_arg);
    return tokens;
}

