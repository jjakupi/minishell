#include "../includes/minishell.h"

void print_tokens(t_token *head)
{
    t_token *current = head;
    if (!current)
    {
        printf("DEBUG: No tokens to print!\n");
        return;
    }
    while (current)
    {
        printf("Token type: %s, value: '%s'\n",
               token_type_to_str(current->type),
               current->value ? current->value : "(null)");
        current = current->next;
    }
}

int main(void)
{
	int last_exit_status = 0;

	while (1)
	{
		char *input = readline(PROMPT);
		if (!input)
		{
			printf("exit\n");
			break;
		}

		// Add to history if input is not empty
		if (*input)
			add_history(input);

		// 🔧 Trigger rl_clear_history for test
		if (strcmp(input, "history -c") == 0)
		{
			rl_clear_history();
			free(input);
			continue;
		}
		// Now continue with your normal pipeline
		t_token *tokens = tokenize(input);
		if (!tokens)
		{
			free(input);
			continue;
		}

		t_command *commands = parse_pipeline(tokens);
		if (!commands)
		{
			free_tokens(tokens);
			free(input);
			continue;
		}

		t_command *cur_cmd = commands;
		while (cur_cmd)
		{
			expand_command_arguments(cur_cmd, last_exit_status);
			cur_cmd = cur_cmd->next;
		}

		cur_cmd = commands;
		while (cur_cmd)
		{
			int status = execute_builtin(cur_cmd);
			if (status == -1)
			{
				fprintf(stderr, "%s: command not found\n", cur_cmd->cmd);
				status = 127;
			}
			last_exit_status = status;
			cur_cmd = cur_cmd->next;
		}

		free_command(commands);
		free_tokens(tokens);
		free(input);
	}
	return 0;
}

