#include "../includes/minishell.h"

char *get_env_value(const char *var) {
    char *value = getenv(var);
    return value ? value : "";
}

char *expand_argument(const char *arg, int last_exit_status)
{
	char buffer[4096] = {0};
	int i = 0, j = 0;

	while (arg[i])
	{
		if (arg[i] == '$')
		{
			if (arg[i + 1] == '?')
			{
				j += sprintf(&buffer[j], "%d", last_exit_status);
				i += 2;
			}
			else if (arg[i + 1] == '$')
			{
				j += sprintf(&buffer[j], "%d", getpid());
				i += 2;
			}
			else if (isalpha(arg[i + 1]) || arg[i + 1] == '_')
			{
				char var_name[256] = {0};
				int k = 0;
				i++;
				while (isalnum(arg[i]) || arg[i] == '_')
					var_name[k++] = arg[i++];
				char *value = getenv(var_name);
				if (value)
					j += sprintf(&buffer[j], "%s", value);
			}
			else
				buffer[j++] = arg[i++];
		}
		else
			buffer[j++] = arg[i++];
	}
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


