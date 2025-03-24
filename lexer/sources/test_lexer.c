#include "../includes/lexer.h"

// Assume free_command_list(t_command *head) is implemented
// and your other parser functions (parse_echo, parse_cd, etc.) are defined.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lexer.h"


int execute_builtin(t_command *cmd);
void expand_command_arguments(t_command *cmd, int last_exit_status);

int main(void)
{
	char input[1024];
	int last_exit_status = 0;

	while (1) // explicitly infinite loop until exit is called
	{
		printf("Minishell: ");
		if (!fgets(input, sizeof(input), stdin))
		{
			printf("\n");
			break; // Exit loop on EOF (Ctrl+D)
		}
		input[strcspn(input, "\n")] = 0; // Remove newline

		if (strlen(input) == 0)
			continue; // skip empty input

		t_token *tokens = tokenize(input);
		if (!tokens)
			continue;

		t_command *commands = parse_pipeline(tokens);
		if (!commands) {
			free_tokens(tokens);
			continue;
		}

		// Explicit expansions (CRITICAL)
		t_command *cur_cmd = commands;
		while (cur_cmd) {
			expand_command_arguments(cur_cmd, last_exit_status);
			cur_cmd = cur_cmd->next;
		}

		// Execute built-ins explicitly (no external commands for now)
		cur_cmd = commands;
        while (cur_cmd)
        {
            int status = execute_builtin(cur_cmd);

             if (status == -1)
            {
              // Command not found, explicitly set status to 127
                fprintf(stderr, "%s: command not found\n", cur_cmd->cmd);
               status = 127;
            }

             last_exit_status = status; // ✅ explicitly update last_exit_status correctly

            cur_cmd = cur_cmd->next;
        }
	}

	return 0;
}
