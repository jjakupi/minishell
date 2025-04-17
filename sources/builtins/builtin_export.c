#include "../include/minishell.h"

/*
** set_env_var:
**   Adds or updates a variable in the environment.
**   'assignment' is of the form "KEY=VALUE" or "KEY". In the latter case,
**   the variable is added with an empty value.
*/
int set_env_var(char ***env_ptr, const char *assignment)
{
	char **env;
	char *eq;
	char key[256];
	int key_len;
	int idx;

	env = *env_ptr;
	eq = strchr(assignment, '=');
	if (eq)
		key_len = eq - assignment;
	else
		key_len = strlen(assignment);
	if (key_len >= (int)sizeof(key))
		key_len = sizeof(key) - 1;
	strncpy(key, assignment, key_len);
	key[key_len] = '\0';
	idx = env_op(env, key, 1);
	if (!eq)
	{
		if (idx == -1)
		{
			int count;

			count = env_op(env, NULL, 0);
			env = realloc(env, (count + 2) * sizeof(char *));
			if (!env)
				return (1);
			env[count] = malloc(key_len + 2);
			if (!env[count])
				return (1);
			strcpy(env[count], key);
			strcat(env[count], "=");
			env[count + 1] = NULL;
			*env_ptr = env;
		}
	}
	else
	{
		char *new_entry;
		int count;

		new_entry = strdup(assignment);
		if (!new_entry)
			return (1);
		if (idx != -1)
		{
			free(env[idx]);
			env[idx] = new_entry;
		}
		else
		{
			count = env_op(env, NULL, 0);
			env = realloc(env, (count + 2) * sizeof(char *));
			if (!env)
			{
				free(new_entry);
				return (1);
			}
			env[count] = new_entry;
			env[count + 1] = NULL;
			*env_ptr = env;
		}
	}
	return (0);
}

int builtin_export(t_command *cmd, char ***env)
{
    int ret = 0;

    // No arguments → print all environment variables
    if (cmd->arg_count == 0)
    {
        print_sorted_env(*env);
        return 0;
    }

    // For each argument (0 through arg_count-1), validate and export
    for (int i = 0; i < cmd->arg_count; i++)
    {
        char *arg = cmd->args[i];

        if (!is_valid_export_token(arg))
        {
            fprintf(stderr, "export: `%s': not a valid identifier\n", arg);
            ret = 1;
        }
        else if (set_env_var(env, arg) != 0)
        {
            // set_env_var failed (e.g. malloc failure)
            ret = 1;
        }
    }
    return ret;
}

