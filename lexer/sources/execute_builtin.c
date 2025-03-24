#include "../includes/minishell.h"

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
	else if (!strcmp(cmd->cmd, "env"))
	{
		extern char **environ;
		for (int i = 0; environ[i]; i++)
			printf("%s\n", environ[i]);
		return 0;
	}
	else if (!strcmp(cmd->cmd, "export"))
	{
		if (cmd->arg_count == 0)
		{
			extern char **environ;
			for (int i = 0; environ[i]; i++)
				printf("declare -x %s\n", environ[i]);
			return 0;
		}
		for (int i = 0; i < cmd->arg_count; i++)
		{
			if (putenv(cmd->args[i]) != 0)
			{
				fprintf(stderr, "export: '%s': not a valid identifier\n", cmd->args[i]);
				return 1;
			}
		}
		return 0;
	}
	else if (!strcmp(cmd->cmd, "unset"))
	{
		for (int i = 0; i < cmd->arg_count; i++)
			unsetenv(cmd->args[i]);
		return 0;
	}
	else if (!strcmp(cmd->cmd, "exit"))
		exit(0);

	return -1; // Not built-in
}
