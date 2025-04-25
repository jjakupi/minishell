#include "../include/minishell.h"

void add_argument(t_command *cmd, const char *arg)
{
    cmd->arg_count++;
    cmd->args = realloc(cmd->args, cmd->arg_count * sizeof(char *));
    if (!cmd->args)
    {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    cmd->args[cmd->arg_count - 1] = ft_strdup(arg);
    if (!cmd->args[cmd->arg_count - 1])
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
}

t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->cmd = NULL;
    cmd->suppress_newline = 0;
    cmd->args = NULL;          // Will be allocated when adding arguments
    cmd->arg_count = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_mode = 0;
    cmd->heredoc_delimiter = NULL;
    cmd->has_heredoc = 0;
    cmd->expand_heredoc = 1;    // or 0, depending on your default behavior
    cmd->next = NULL;
    return cmd;
}

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

void normalize_empty_cmd(t_command *c)
{
    if (c->cmd[0] == '\0' && c->arg_count > 0)
    {
        free(c->cmd);
        c->cmd = strdup(c->args[0]);
        for (int i = 1; i < c->arg_count; i++)
            c->args[i-1] = c->args[i];
        c->arg_count--;
        c->args = realloc(c->args, (c->arg_count+1) * sizeof *c->args);
        c->args[c->arg_count] = NULL;
    }
}
