#include "../include/minishell.h"

/*
** env_op:
**   If mode == 0, returns the number of elements in env.
**   If mode == 1, returns the index of the variable whose key matches 'key'
**   (comparison up to the '=' character), or -1 if not found.
*/
int env_op(char **env, const char *key, int mode)
{
	int i;
	int key_len;

	i = 0;
	if (mode == 0)
	{
		while (env[i])
			i++;
		return (i);
	}
	else if (mode == 1)
	{
		key_len = strlen(key);
		while (env[i])
		{
			if (strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
				return (i);
			i++;
		}
		return (-1);
	}
	return (-1);
}

/*
** is_valid_identifier_export:
**   Returns 1 if token is a valid identifier for export,
**   i.e. it does not start with '=' or a digit and does not contain
**   single or double quotes; otherwise returns 0.
*/
int is_valid_identifier_export(const char *token)
{
	if (!token || token[0] == '=' || isdigit(token[0]) ||
		strchr(token, '\'') || strchr(token, '"'))
		return (0);
	return (1);
}

/*
** cmpfunc
**   Compare two strings (pointeurs sur char) pour qsort.
*/
int cmpfunc(const void *a, const void *b)
{
	const char *s1;
	const char *s2;

	s1 = *(const char **)a;
	s2 = *(const char **)b;
	return (int)strcmp(s1, s2);
}

/*
** print_sorted_env:
**   Prints the environment variables sorted in lexicographical order,
**   using the format:
**     declare -x KEY="VALUE"  (if variable is assigned)
**     declare -x KEY          (if not)
*/
void print_sorted_env(char **env)
{
    int count = env_op(env, NULL, 0);
    char **env_copy = malloc((count + 1) * sizeof(char *));
    if (!env_copy)
        return;

    for (int i = 0; i < count; i++)
        env_copy[i] = env[i];
    env_copy[count] = NULL;

    qsort(env_copy, count, sizeof(char *), cmpfunc);

    for (int i = 0; i < count; i++)
    {
        char *var = env_copy[i];
        char *eq  = strchr(var, '=');

        if (eq)
        {
            int key_len = eq - var;
            if (key_len >= 256) key_len = 255;

            char key[256];
            strncpy(key, var, key_len);
            key[key_len] = '\0';

            // If the value is empty, omit =""
            if (*(eq + 1) == '\0')
                printf("declare -x %s\n", key);
            else
                printf("declare -x %s=\"%s\"\n", key, eq + 1);
        }
        else
        {
            // No '=' means no value at all
            printf("declare -x %s\n", var);
        }
    }

    free(env_copy);
}

