#include "../include/minishell.h"

int execute_builtin(t_command *cmd)
{
	if (!cmd || !cmd->cmd)
		return -1;

	if (!strcmp(cmd->cmd, "echo"))
	{
		int i = 0;
		int suppress_newline = 0;

		if (cmd->arg_count > 0 && !strcmp(cmd->args[0], "-n"))
		{
			suppress_newline = 1;
			i++;
		}

		for (; i < cmd->arg_count; i++)
		{
			printf("%s", cmd->args[i]);
			if (i < cmd->arg_count - 1)
				printf(" ");
		}

		if (!suppress_newline)
			printf("\n");

		return 0;
	}

	// Placeholder for additional built-in commands
	// e.g., if (!strcmp(cmd->cmd, "cd")) { ... }
	//       if (!strcmp(cmd->cmd, "pwd")) { ... }

	return -1; // Command not found among built-ins
}
