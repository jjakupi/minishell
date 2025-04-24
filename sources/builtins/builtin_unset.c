#include "../include/minishell.h"

/*
** env_len
**   Returns the number of strings in the environment array.
*/
int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

/*
** env_idx
**   Returns the index of the environment variable whose key matches 'key'.
**   Comparison is done up to '=' or end-of-string.
*/
int	env_idx(char **env, const char *key)
{
	int		i;
	int		key_len;

	i = 0;
	key_len = (int)strlen(key);
	while (env[i])
	{
		if (strncmp(env[i], key, key_len) == 0 &&
			(env[i][key_len] == '=' || env[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

/*
** env_remove_var
**   Removes the environment variable with the given key from the env array.
**   Shifts the remaining entries and frees the removed string.
**   Returns 0 on success.
*/
int	env_remove_var(char ***env_ptr, const char *key)
{
	char	**env;
	int		idx;
	int		len;
	int		i;

	env = *env_ptr;
	idx = env_idx(env, key);
	if (idx == -1)
		return (0);
	free(env[idx]);
	len = env_len(env);
	i = idx;
	while (i < len - 1)
	{
		env[i] = env[i + 1];
		i++;
	}
	env[len - 1] = NULL;
	return (0);
}

/*
** valid_unset_identifier
**   Verifies that token is a valid identifier for unset.
**   It must not be empty, must not start with '=' or a digit.
**   Returns 1 if valid, or 0 otherwise.
*/
int valid_unset_identifier(const char *s)
{
    if (!s || !(ft_isalpha(s[0]) || s[0] == '_'))
        return 0;
    for (int i = 1; s[i]; i++)
        if (!(ft_isalnum(s[i]) || s[i] == '_'))
            return 0;
    return 1;
}


/*
** ms_builtin_unset
**   Implements the unset builtin.
**   For each argument in cmd->args (starting at index 1), it checks the validity;
**   if valid, it removes the variable from the environment.
**   Prints an error message for any invalid identifier.
**   Returns EXIT_SUCCESS if all processed, or EXIT_FAILURE if any error occurred.
*/
int builtin_unset(t_command *cmd, char ***env)
{
    int ret = EXIT_SUCCESS;

    // Nothing to do if no args
    if (cmd->arg_count == 0)
        return ret;

    // Loop from 0 to arg_count-1
    for (int i = 0; i < cmd->arg_count; i++)
    {
        char *name = cmd->args[i];

        if (!valid_unset_identifier(name))
        {
            fprintf(stderr, "unset: `%s': not a valid identifier\n", name);
            ret = EXIT_FAILURE;
        }
        else
        {
            env_remove_var(env, name);
        }
    }

    return ret;
}
