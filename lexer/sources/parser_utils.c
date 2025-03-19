#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    cmd->cmd = NULL;
    cmd->suppress_newline = 0;
    cmd->args = NULL;
    cmd->arg_count = 0;
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
    free(cmd);
}

int is_valid_identifier(const char *str)
{
    if (!str || !*str)
        return 0;
    // The first character must be a letter or underscore.
    if (!ft_isalpha(*str) && *str != '_')
        return 0;
    // Subsequent characters must be alphanumeric or underscore.
    for (int i = 1; str[i]; i++) {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
    }
    return 1;
}
