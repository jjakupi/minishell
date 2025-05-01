#include "../include/minishell.h"

extern char **environ;

void minishell_perror(const char *what)
{
    const char *msg = strerror(errno);
    write(2, "minishell: ", 11);
    write(2, what, strlen(what));
    write(2, ": ", 2);
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
}

char *find_executable(const char *name)
{
    char *path_env = getenv("PATH");
    if (!path_env) return NULL;
    char *paths = strdup(path_env);
    for (char *dir = strtok(paths, ":"); dir; dir = strtok(NULL, ":"))
    {
        char buf[1024];
        snprintf(buf, sizeof(buf), "%s/%s", dir, name);
        if (access(buf, X_OK) == 0)
        {
            free(paths);
            return strdup(buf);
        }
    }
    free(paths);
    return NULL;
}


static int exec_single(t_command *cmd)
{
    pid_t pid = fork();
    if (pid < 0) { minishell_perror("fork"); return 1; }
    if (pid == 0)
        child_exec_one(cmd, -1, -1);
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int execute_command(t_command *cmd)
{
    bool pipeline           = (cmd->next != NULL);
    bool standalone_builtin = (!pipeline && is_builtin(cmd->cmd));
    int status;

    // Empty‐string => no-op
    if (!cmd->cmd || cmd->cmd[0] == '\0')
        return 0;

    if (standalone_builtin)
    {
        int saved_in  = -1, saved_out = -1;
        int fd;

        // 1) ALL output redirs > & >>
        for (int i = 0; i < cmd->out_count; i++)
        {
            int flags = O_WRONLY | O_CREAT |
                       (cmd->append_flags[i] ? O_APPEND : O_TRUNC);
            fd = open(cmd->out_files[i], flags, 0644);
            if (fd < 0)
            {
                minishell_perror(cmd->out_files[i]);
                return 1;
            }
            saved_out = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // 2) ALL input redirs <
        for (int i = 0; i < cmd->in_count; i++)
        {
            fd = open(cmd->in_files[i], O_RDONLY);
            if (fd < 0)
            {
                minishell_perror(cmd->in_files[i]);
                if (saved_out >= 0) { dup2(saved_out, STDOUT_FILENO); close(saved_out); }
                return 1;
            }
            saved_in = dup(STDIN_FILENO);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // 3) run builtin
        status = execute_builtin(cmd);

        // 4) restore stdio
        if (saved_in  >= 0) { dup2(saved_in,  STDIN_FILENO);  close(saved_in);  }
        if (saved_out >= 0) { dup2(saved_out, STDOUT_FILENO); close(saved_out); }

        return status;
    }

    // pipeline or single external
    if (pipeline)
        status = exec_pipeline(cmd);
    else
        status = exec_single(cmd);

    return status;
}
