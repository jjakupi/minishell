#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

char *get_env_value(const char *var) {
    char *value = getenv(var);
    return value ? value : "";
}

char *expand_argument(const char *arg, int last_exit_status) {
    char buffer[8192] = {0};
    int i = 0, j = 0;

    while (arg[i]) {
        if (arg[i] == '$') {
            if (arg[i + 1] == '?') {
                char status[16];
                sprintf(status, "%d", last_exit_status);
                strcpy(&buffer[j], status);
                j += strlen(status);
                i += 2;
            }
            else if (arg[i + 1] == '$') {
                char pid[16];
                sprintf(pid, "%d", getpid());
                strcpy(&buffer[j], pid);
                j += strlen(pid);
                i += 2;
            }
            else if (!arg[i + 1] || strchr(" \"'$", arg[i + 1])) {
                buffer[j++] = '$';
                i++;
            }
            else {
                char var_name[256] = {0};
                int k = 0;
                i++;
                while (arg[i] && (isalnum(arg[i]) || arg[i] == '_'))
                    var_name[k++] = arg[i++];
                var_name[k] = '\0';

                char *env_val = get_env_value(var_name);
                strcpy(&buffer[j], env_val);
                j += strlen(env_val);
            }
        }
        else {
            buffer[j++] = arg[i++];
        }
    }
    buffer[j] = '\0';
    return strdup(buffer);
}

// Possible incorrect version of your expansion function
void expand_command_arguments(t_command *cmd, int last_exit_status)
{
    if (cmd->cmd)
    {
        char *expanded_cmd = expand_argument(cmd->cmd, last_exit_status);
        free(cmd->cmd);
        cmd->cmd = expanded_cmd;
    }

    // 👇 Ensure this part is exactly as follows (correct):
    for (int i = 0; i < cmd->arg_count; i++)
    {
        char *expanded = expand_argument(cmd->args[i], last_exit_status);
        free(cmd->args[i]);
        cmd->args[i] = expanded;
    }
}


