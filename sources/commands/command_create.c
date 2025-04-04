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
