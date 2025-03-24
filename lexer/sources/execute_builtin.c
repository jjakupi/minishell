#include "../includes/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int execute_builtin(t_command *cmd)
{
	if (!strcmp(cmd->cmd, "echo"))
	{
		int i = 0;
		if (cmd->arg_count > 0 && !strcmp(cmd->args[0], "-n"))
			i = 1, cmd->suppress_newline = 1;

		for (; i < cmd->arg_count; i++)
			printf("%s%s", cmd->args[i], (i < cmd->arg_count - 1) ? " " : "");
		if (!cmd->suppress_newline)
			printf("\n");
		return 0;
	}
	else if (!strcmp(cmd->cmd, "cd"))
	{
		char *path = cmd->args[0] ? cmd->args[0] : getenv("HOME");
		if (chdir(path) == -1)
		{
			perror("cd");
			return 1;
		}
		return 0;
	}
	else if (!strcmp(cmd->cmd, "pwd"))
	{
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
		else
			perror("pwd");
		return 0;
	}
	else if (!strcmp(cmd->cmd, "exit"))
		exit(0);

	return -1; // Not built-in
}
