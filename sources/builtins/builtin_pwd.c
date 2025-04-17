#include "../include/minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[MAX_PATH];
	(void)	cmd;
	
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	return (EXIT_SUCCESS);
}

