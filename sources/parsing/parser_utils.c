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
int	is_operator(int type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

t_command *parse_single_command(t_token *tokens)
{
	if (!tokens)
		return NULL;

	t_command *cmd = NULL;

	if (!strcmp(tokens->value, "cd"))
		cmd = parse_cd(tokens);
	else if (!strcmp(tokens->value, "export"))
		cmd = parse_export(tokens);
	else if (!strcmp(tokens->value, "unset"))
		cmd = parse_unset(tokens);
	else if (!strcmp(tokens->value, "env"))
		cmd = parse_env(tokens);
	else if (!strcmp(tokens->value, "exit"))
		cmd = parse_exit(tokens);
	else if (!strcmp(tokens->value, "echo"))
		cmd = parse_echo(tokens);
	else if (!strcmp(tokens->value, "pwd"))
		cmd = parse_pwd(tokens);
	else
	{
		cmd = create_command();
		if (!cmd)
			return NULL;

		cmd->cmd = ft_strdup(tokens->value);
		if (!cmd->cmd)
		{
			free_command(cmd);
			return NULL;
		}

		tokens = tokens->next;
		while (tokens)
		{
			if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT ||
			    tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
			{
				if (!tokens->next)
				{
					printf("minishell: syntax error near unexpected token `newline'\n");
					free_command(cmd);
					return NULL;
				}
				else if (tokens->next->type == PIPE || tokens->next->type == REDIR_IN ||
						 tokens->next->type == REDIR_OUT || tokens->next->type == REDIR_APPEND ||
						 tokens->next->type == HEREDOC)
				{
					printf("minishell: syntax error near unexpected token `%s'\n", tokens->next->value);
					free_command(cmd);
					return NULL;
				}
				else
				{
					if ((tokens->type == REDIR_OUT && parse_output_redirection(cmd, &tokens) == -1) ||
						(tokens->type == REDIR_APPEND && parse_append_redirection(cmd, &tokens) == -1) ||
						(tokens->type == REDIR_IN && parse_input_redirection(cmd, &tokens) == -1) ||
						(tokens->type == HEREDOC && parse_heredoc(cmd, &tokens) == -1))
					{
						free_command(cmd);
						return NULL;
					}
					continue;
				}
			}
			else if (tokens->type == WORD || tokens->type == ENV_VAR)
			{
				add_argument(cmd, tokens->value);
				tokens = tokens->next;
			}
			else if (tokens->type == PIPE)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				free_command(cmd);
				return NULL;
			}
			else
				tokens = tokens->next;
		}
		return cmd;
	}

	return cmd;
}
