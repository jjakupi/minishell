#include "../include/minishell.h"

int	builtin_exit(t_command *cmd)
{
	int	exit_code;

	printf("exit\n");
	if (cmd->arg_count == 0)
		exit(EXIT_SUCCESS);
	if (!is_numeric(cmd->args[0]))
	{
		printf("minishell: exit: %s: numeric argument required\n", cmd->args[0]);
		exit(255);
	}
	if (cmd->arg_count > 1)
	{
		printf("minishell: exit: too many arguments\n");
		return (1); // do NOT exit, return explicitly error status
	}
	exit_code = ft_atoi(cmd->args[0]);
	exit((unsigned char)exit_code);
}
