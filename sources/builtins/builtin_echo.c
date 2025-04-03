#include "../include/minishell.h"

int builtin_echo(t_command *cmd)
{
	int	i;
	int	suppress_newline;

	if (!cmd || !cmd->cmd)
		return (-1);

	suppress_newline = 0;
	i = 0;

	// Check explicitly for "-n" flag
	if (cmd->arg_count > 0 && ft_strcmp(cmd->args[0], "-n") == 0)
	{
		suppress_newline = 1;
		i++;
	}

	// Print arguments explicitly with spaces
	while (i < cmd->arg_count)
	{
		printf("%s", cmd->args[i]);
		if (i < cmd->arg_count - 1)
			printf(" ");
		i++;
	}

	// Explicitly handle newline
	if (!suppress_newline)
		printf("\n");

	return (0);
}

