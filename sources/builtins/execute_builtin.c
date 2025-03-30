#include "../include/minishell.h"

// Check if command is a built-in
int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
            !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
            !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit"));
}

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

	// Recognize your future built-ins here explicitly
	if (!strcmp(cmd->cmd, "cd") || !strcmp(cmd->cmd, "pwd") ||
		!strcmp(cmd->cmd, "export") || !strcmp(cmd->cmd, "unset") ||
		!strcmp(cmd->cmd, "exit"))
	{
		printf("[DEBUG] Built-in '%s' recognized but not yet implemented\n", cmd->cmd);
		return 0;
	}

	fprintf(stderr, "minishell: %s: command not found\n", cmd->cmd);
	return 127;
}

