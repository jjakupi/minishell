#include "../include/minishell.h"

void free_command(t_command *cmd)
{
    if (!cmd)
        return;

    if (cmd->cmd)
        free(cmd->cmd);

    if (cmd->args)
    {
        for (int i = 0; i < cmd->arg_count; i++)
        {
            if (cmd->args[i])
                free(cmd->args[i]);
        }
        free(cmd->args);
    }

    if (cmd->input_file)
        free(cmd->input_file);

    if (cmd->output_file)
        free(cmd->output_file);

    if (cmd->heredoc_delimiter)
        free(cmd->heredoc_delimiter);

    // Do not forget to free the next command if this is part of a linked list.
    if (cmd->next)
        free_command(cmd->next);

    free(cmd);
}
