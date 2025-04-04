#include "../include/minishell.h"

t_command *parse_single_command(t_token *tokens)
{
    if (!tokens)
        return NULL;

    t_command *cmd = create_command();
    if (!cmd)
        return NULL;

    cmd->cmd = ft_strdup(tokens->value);
    tokens = tokens->next;

    while (tokens)
    {
        if (tokens->type == WORD || tokens->type == ENV_VAR)
        {
            add_argument(cmd, tokens->value);
            tokens = tokens->next;
        }
        else if (tokens->type == REDIR_IN)
        {
            if (parse_input_redirection(cmd, &tokens) == -1)
            {
                free_command(cmd);
                return NULL;
            }
        }
        else if (tokens->type == REDIR_OUT)
        {
            if (parse_output_redirection(cmd, &tokens) == -1)
            {
                free_command(cmd);
                return NULL;
            }
        }
        else if (tokens->type == REDIR_APPEND)
        {
            if (parse_append_redirection(cmd, &tokens) == -1)
            {
                free_command(cmd);
                return NULL;
            }
        }
        else if (tokens->type == HEREDOC)
        {
            if (parse_heredoc(cmd, &tokens) == -1)
            {
                free_command(cmd);
                return NULL;
            }
        }
        else if (tokens->type == PIPE)
        {
            fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
            free_command(cmd);
            return NULL;
        }
        else
            tokens = tokens->next;
    }

    return cmd;
}
