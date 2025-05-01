#include "../include/minishell.h"

#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Add one argument—keep args NULL-terminated */
void add_argument(t_command *cmd, const char *arg)
{
    // make room for one more + a NULL sentinel
    char **tmp = realloc(cmd->args,
                        sizeof *tmp * (cmd->arg_count + 2));
    if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
    cmd->args = tmp;

    // strdup the new argument
    cmd->args[cmd->arg_count] = ft_strdup(arg);
    if (!cmd->args[cmd->arg_count]) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    cmd->arg_count++;
    cmd->args[cmd->arg_count] = NULL;  // NULL terminate
}

/* Create a fresh command with every pointer/count zeroed */
t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof *cmd);
    if (!cmd) return NULL;

    cmd->cmd               = NULL;
    cmd->suppress_newline  = 0;
    cmd->args              = NULL;
    cmd->arg_count         = 0;

    cmd->in_files          = NULL;
    cmd->in_count          = 0;
    cmd->out_files         = NULL;
    cmd->out_count         = 0;
    cmd->append_flags      = NULL;

    cmd->heredoc_delimiter = NULL;
    cmd->has_heredoc       = 0;
    cmd->expand_heredoc    = 0;

    cmd->next              = NULL;
    return cmd;
}

void free_command(t_command *cmd)
{
    if (!cmd) return;

    free(cmd->cmd);
    for (int i = 0; i < cmd->arg_count; i++)
        free(cmd->args[i]);
    free(cmd->args);

    for (int i = 0; i < cmd->in_count; i++)
        free(cmd->in_files[i]);
    free(cmd->in_files);

    for (int i = 0; i < cmd->out_count; i++)
        free(cmd->out_files[i]);
    free(cmd->out_files);
    free(cmd->append_flags);

    free(cmd->heredoc_delimiter);

    free_command(cmd->next);
    free(cmd);
}

/* (Optional) normalize empty commands as you had before */
void normalize_empty_cmd(t_command *c)
{
    if (!c->cmd) return;
    if (c->cmd[0] == '\0' && c->arg_count > 0)
    {
        free(c->cmd);
        c->cmd = strdup(c->args[0]);
        for (int i = 1; i < c->arg_count; i++)
            c->args[i-1] = c->args[i];
        c->arg_count--;
        c->args[c->arg_count] = NULL;
        // shrink the args array if you want:
        // c->args = realloc(c->args, sizeof *c->args*(c->arg_count+1));
    }
}
