#include "../include/minishell.h"

int builtin_exit(t_command *cmd)
{
    long code;

    printf("exit\n");
    if (cmd->arg_count == 0)
        exit(EXIT_SUCCESS);

    if (!is_numeric(cmd->args[0]))
    {
        fprintf(stderr, "bash: exit: %s: numeric argument required\n",
                cmd->args[0]);
        exit(255);
    }

    if (cmd->arg_count > 1)
    {
        fprintf(stderr, "bash: exit: too many arguments\n");
        return 1;   // do NOT exit; return status 1
    }

    code = ft_atoi(cmd->args[0]);
    exit((unsigned char)code);
}
