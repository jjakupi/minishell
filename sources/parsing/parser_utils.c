#include "../include/minishell.h"

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
        return NULL;
    cmd->cmd = NULL;
    cmd->suppress_newline = 0;
    cmd->args = NULL;          // Will be allocated when adding arguments
    cmd->arg_count = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_mode = 0;
    cmd->heredoc_delimiter = NULL;
    cmd->has_heredoc = 0;
    cmd->expand_heredoc = 1;    // or 0, depending on your default behavior
    cmd->next = NULL;
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

    if (cmd->input_file)
        free(cmd->input_file);

    if (cmd->output_file)
        free(cmd->output_file);

    if (cmd->heredoc_delimiter)
        free(cmd->heredoc_delimiter);

    // Do not forget to free the next command if this is part of a linked list.
    if (cmd->next)
        free_command(cmd->next);

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

int	has_unmatched_quotes(const char *str)
{
	char	quote = 0;
	for (int i = 0; str[i]; i++)
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
	}
	return (quote != 0);
}

t_command *parse_single_command(t_token *tokens)
{
	if (!tokens || !tokens->value)
		return NULL;

	t_command *cmd = NULL;

	// Handle built-in commands explicitly first
	if (strcmp(tokens->value, "echo") == 0)
		cmd = parse_echo(tokens);
	else if (strcmp(tokens->value, "cd") == 0)
		cmd = parse_cd(tokens);
	else if (strcmp(tokens->value, "pwd") == 0)
		cmd = parse_pwd(tokens);
	else if (strcmp(tokens->value, "export") == 0)
		cmd = parse_export(tokens);
	else if (strcmp(tokens->value, "unset") == 0)
		cmd = parse_unset(tokens);
	else if (strcmp(tokens->value, "env") == 0)
		cmd = parse_env(tokens);
	else if (strcmp(tokens->value, "exit") == 0)
		cmd = parse_exit(tokens);
	else
	{
		cmd = create_command();
		if (!cmd)
			return NULL;

		char buffer[4096];
		t_token *cur = tokens;

		// Merge adjacent WORD and ENV_VAR tokens explicitly into separate arguments with spaces
		while (cur)
		{
			if (cur->type == WORD || cur->type == ENV_VAR)
			{
				buffer[0] = '\0'; // reset buffer for each argument
				while (cur && (cur->type == WORD || cur->type == ENV_VAR))
				{
					if (buffer[0] != '\0')
						strcat(buffer, " "); // explicitly add a space between tokens
					strcat(buffer, cur->value);
					cur = cur->next;
				}
				if (!cmd->cmd)
					cmd->cmd = ft_strdup(buffer);
				add_argument(cmd, buffer);
			}
			else if (cur->type == REDIR_IN)
			{
				if (parse_input_redirection(cmd, &cur) == -1)
				{
					free_command(cmd);
					return NULL;
				}
			}
			else if (cur->type == REDIR_OUT || cur->type == REDIR_APPEND)
			{
				if (parse_output_redirection(cmd, &cur) == -1)
				{
					free_command(cmd);
					return NULL;
				}
			}
			else if (cur->type == HEREDOC)
			{
				if (parse_heredoc(cmd, &cur) == -1)
				{
					free_command(cmd);
					return NULL;
				}
			}
			else
				cur = cur->next;
		}
	}

	return cmd;
}
