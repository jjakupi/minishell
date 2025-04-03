#include "../include/minishell.h"

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
		return (-1);

	if (!ft_strcmp(cmd->cmd, "echo"))
		return (builtin_echo(cmd));
	else if (!ft_strcmp(cmd->cmd, "exit"))
		return (builtin_exit(cmd));

	printf("minishell: %s: command not found\n", cmd->cmd);
	return (127);
}


