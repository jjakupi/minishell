#include "../include/minishell.h"

int builtin_cd(t_command *cmd)
{
    char cwd[MAX_PATH];
    char *path;

    if (cmd->arg_count > 1)
    {
        printf("minishell: cd: too many arguments\n");
        return EXIT_FAILURE;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("cd: getcwd error");
        return EXIT_FAILURE;
    }

    if (cmd->arg_count == 0 || strcmp(cmd->args[0], "~") == 0)
    {
        path = getenv("HOME");
        if (!path)
        {
            ft_putstr_fd("cd: HOME not set\n", 2);
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(cmd->args[0], "-") == 0)
    {
        path = getenv("OLDPWD");
        if (!path)
        {
            ft_putstr_fd("cd: OLDPWD not set\n", 2);
            return EXIT_FAILURE;
        }
        printf("%s\n", path);
    }
    else if (cmd->args[0][0] == '\0')
    {
        ft_putstr_fd("cd: empty path\n", 2);
        return EXIT_FAILURE;
    }
    else
        path = cmd->args[0];

    if (chdir(path) != 0)
    {
        perror("cd");
        return EXIT_FAILURE;
    }
    return update_directories(cwd);
}
