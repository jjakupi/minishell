#include "../include/minishell.h"

int main(void)
{
	char *input = NULL;
	t_token *tokens = NULL;
	t_command *cmds = NULL;
	int parse_status;
	int last_exit_status = 0;
    (void)last_exit_status;
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			printf("exit\n");
			break;
		}

		if (*input)
			add_history(input);

		tokens = tokenize(input);
		if (!tokens)
		{
			free(input);
			continue;
		}

		parse_status = parse_pipeline(tokens, &cmds);
		if (parse_status == 2 || !cmds)
		{
			free_tokens(tokens);
			tokens = NULL;
			free(input);
			input = NULL;
			last_exit_status = 2;
			continue;
		}

		for (t_command *cmd = cmds; cmd; cmd = cmd->next)
		{
			printf("[COMMAND] Name: '%s'\n", cmd->cmd);
			for (int x = 0; x < cmd->arg_count; x++)
				printf("[ARG %d]: '%s'\n", x, cmd->args[x]);
			if (cmd->input_file)
				printf("[INPUT FILE]: '%s'\n", cmd->input_file);
			if (cmd->output_file)
				printf("[OUTPUT FILE]: '%s' (append: %d)\n",
						cmd->output_file, cmd->append_mode);
			if (cmd->has_heredoc)
				printf("[HEREDOC]: '%s'\n", cmd->heredoc_delimiter);
		}

		if (cmds)
			last_exit_status = execute_builtin(cmds);

		free_command(cmds);
		free_tokens(tokens);
		free(input);
	}
	return 0;
}


char *remove_surrounding_quotes(const char *str)
{
    size_t len = strlen(str);
    if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') ||
                     (str[0] == '\'' && str[len - 1] == '\'')))
        return ft_substr(str, 1, len - 2);
    return strdup(str);
}
