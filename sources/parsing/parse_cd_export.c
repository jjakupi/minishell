#include "../include/minishell.h"

t_command *parse_cd(t_token *tokens)
{
	if (!tokens || ft_strcmp(tokens->value, "cd") != 0)
	{
		printf("minishell: Error: Expected cd command\n");
		return NULL;
	}

	t_command *command = create_command();
	if (!command)
		return NULL;

	command->cmd = ft_strdup("cd");
	if (!command->cmd)
	{
		free_command(command);
		return NULL;
	}

	t_token *current = tokens->next; // explicitly move past "cd"
	if (!current)
	{
		char *home = getenv("HOME");
		if (home)
			add_argument(command, home);
		return command;
	}
	// Explicit syntax check for redirection or pipe tokens as first argument
	if (current->type == PIPE || current->type == REDIR_IN ||
		current->type == REDIR_OUT || current->type == REDIR_APPEND ||
		current->type == HEREDOC)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		free_command(command);
		return NULL;
	}

	// Check explicitly if too many arguments
	if (current->next)
	{
		printf("minishell: cd: too many arguments\n");
		free_command(command);
		return NULL;
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
