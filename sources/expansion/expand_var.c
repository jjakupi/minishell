#include "../include/minishell.h"

char *expand_argument(const char *arg, int last_exit_status)
{
    printf("[DEBUG] BEFORE expansion: '%s'\n", arg); // debugging explicitly
    char buffer[4096] = {0};
    int i = 0, j = 0;
    int len = strlen(arg);

    // If clearly single quoted, return literally
    if (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'')
    {
        strncpy(buffer, arg + 1, len - 2);
        buffer[len - 2] = '\0';
        return strdup(buffer);
    }

    while (arg[i])
    {
        if (arg[i] == '$')
        {
            if (arg[i + 1] == '?')
            {
                j += sprintf(buffer + j, "%d", last_exit_status);
                i += 2;
            }
            else if (arg[i + 1] == '$')
            {
                j += sprintf(buffer + j, "%d", getpid());
                i += 2;
            }
            else if (isalpha(arg[i + 1]) || arg[i + 1] == '_')
            {
                char var_name[256] = {0};
                int k = 0;
                i++;
                while (isalnum(arg[i]) || arg[i] == '_')
                    var_name[k++] = arg[i++];
                char *env_value = getenv(var_name);
                if (env_value)
                    j += sprintf(buffer + j, "%s", env_value);
            }
            else
                buffer[j++] = arg[i++];
        }
        else if (arg[i] != '\'' && arg[i] != '"')
            buffer[j++] = arg[i++];
        else
            i++;
    }

    buffer[j] = '\0';
    return strdup(buffer);
}


// Corrected command arguments expansion
void expand_command_arguments(t_command *cmd, int last_exit_status)
{
	if (!cmd)
		return;

	// Expand command itself (though usually not required for builtins)
	if (cmd->cmd)
	{
		char *expanded_cmd = expand_argument(cmd->cmd, last_exit_status);
		free(cmd->cmd);
		cmd->cmd = expanded_cmd;
	}

	// Expand each argument properly
	for (int i = 0; i < cmd->arg_count; i++)
	{
		char *expanded = expand_argument(cmd->args[i], last_exit_status);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
	}
}
