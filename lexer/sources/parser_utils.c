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

// Check if a token is a valid export token.
// Since your lexer already handles quotes, the token value is assumed to be unquoted.
// It can be either a valid identifier or an assignment NAME=VALUE,
// where NAME is a valid identifier.
int is_valid_export_token(const char *str)
{
    if (!str)
        return 0;

    char *equal_sign = ft_strchr(str, '=');
    if (equal_sign) {
        // Extract the substring before '='.
        size_t name_len = equal_sign - str;
        // Temporarily duplicate the identifier part.
        char *name = strndup(str, name_len);
        if (!name)
            return 0;
        int valid = is_valid_identifier(name);
        free(name);
        return valid;
    } else {
        return is_valid_identifier(str);
    }
}

int is_numeric(const char *str)
{
    if (!str || !*str)
        return 0;
    if (*str == '+' || *str == '-')
        str++;
    if (!*str)
        return 0;
    while (*str) {
        if (!ft_isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}
