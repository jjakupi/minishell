#include "../include/minishell.h"

int parse_heredoc(t_command *cmd, t_token **current)
{
    char *value;

    if (check_next_token(*current, &value) == -1)
        return -1;

    if (has_unmatched_quotes(value))
        return 0;  // Silent bash-like behavior

    if (cmd->heredoc_delimiter)
        free(cmd->heredoc_delimiter);

    cmd->heredoc_delimiter = ft_strdup(value);
    if (!cmd->heredoc_delimiter)
    {
        perror("strdup");
        return -1;
    }

    cmd->has_heredoc = 1;
    cmd->expand_heredoc = (value[0] != '\'' && value[0] != '"');

    *current = (*current)->next->next;
    return 0;
}

int parse_append_redirection(t_command *cmd, t_token **current)
{
    char *value;

    if (check_next_token(*current, &value) == -1)
        return -1;

    if (cmd->output_file)
        free(cmd->output_file);

    cmd->output_file = strdup(value);
    if (!cmd->output_file)
    {
        perror("strdup");
        return -1;
    }
    cmd->append_mode = 1;

    *current = (*current)->next->next;
    return 0;
}
