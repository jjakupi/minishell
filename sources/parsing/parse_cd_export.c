#include "../include/minishell.h"

t_command *parse_cd(t_token *tokens)
{
	if (!tokens || ft_strcmp(tokens->value, "cd") != 0)
	{
		fprintf(stderr, "Error: Expected cd command\n");
		return NULL;
	}

	t_command *command = create_command();
	command->cmd = ft_strdup("cd");

	t_token *current = tokens->next; // Move past "cd"

	// If no argument is provided, use HOME environment variable
	if (!current)
	{
		char *home = getenv("HOME");
		if (home)
			add_argument(command, home);
		return command;
	}

	// If more than one argument, explicitly return NULL to indicate error clearly!
	if (current->next)
	{
		fprintf(stderr, "cd: too many arguments\n");
		free_command(command); // explicitly free the allocated command
		return NULL;           // return NULL explicitly after error
	}

	// Accept both WORD and ENV_VAR tokens here explicitly
	if (current->type != WORD && current->type != ENV_VAR)
	{
		fprintf(stderr, "cd: invalid argument\n");
		free_command(command); // explicitly free the allocated command
		return NULL;           // return NULL explicitly after error
	}

	add_argument(command, current->value);

	return command;
}

t_command *parse_export(t_token *tokens)
{
    if (!tokens || strcmp(tokens->value, "export") != 0)
    {
        fprintf(stderr, "Error: Expected export command\n");
        return NULL;
    }

    t_command *command = create_command();
    command->cmd = strdup("export");

    t_token *current = tokens->next; // Skip the "export" token

    while (current)
    {
        if (current->type == WORD)
        {
            // Validate the token (e.g. "VAR=HELLO")
            if (!is_valid_export_token(current->value))
            {
                fprintf(stderr, "export: `%s': not a valid identifier\n", current->value);
                free_command(command);
                return NULL;
            }
            // If valid, add it to the command's arguments (or ignore it if you prefer).
            add_argument(command, current->value);
        }
        current = current->next;
    }
    return command;
}
