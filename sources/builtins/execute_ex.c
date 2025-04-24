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

// 1) Find an executable on $PATHmini
static char *find_executable(const char *name)
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

#include <sys/stat.h>
#include <errno.h>
// … other includes …

static void child_exec_one(t_command *cmd)
{
    // … your existing heredoc + redir + builtin handling …

    // build argv[]
    char **argv = calloc(cmd->arg_count + 2, sizeof(char *));
    argv[0] = cmd->cmd;
    for (int i = 0; i < cmd->arg_count; i++)
        argv[i+1] = cmd->args[i];
    argv[cmd->arg_count+1] = NULL;

    // pick a path
    char *path;
    if (strchr(cmd->cmd, '/')) {
        path = cmd->cmd;
    } else {
        path = find_executable(cmd->cmd);
        if (!path) {
            write(2, "minishell: ", 11);
            write(2, cmd->cmd, strlen(cmd->cmd));
            write(2, ": command not found\n", 20);
            exit(127);
        }
    }

    // ←– INSERTED: check for directory
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        write(2, "minishell: ", 11);
        write(2, cmd->cmd, strlen(cmd->cmd));
        write(2, ": Is a directory\n", 17);
        exit(126);
    }

    // now do the real execve
    execve(path, argv, environ);

    // if execve() returns, it failed for some other reason
    perror(cmd->cmd);
    exit(errno == EACCES ? 126 : 127);
}




// 3) Chain N>1 commands into a pipeline
int exec_pipeline(t_command *head)
{
    t_command *c = head;
    int in_fd = STDIN_FILENO, n = 0;
    pid_t pids[64];

    // for all but last, fork + pipe
    for (; c->next; c = c->next, n++)
    {
        int fds[2];
        pipe(fds);
        if ((pids[n] = fork()) == 0)
        {
            dup2(in_fd, STDIN_FILENO);
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]); close(fds[1]);
            if (in_fd != STDIN_FILENO) close(in_fd);
            child_exec_one(c);
        }
        close(fds[1]);
        if (in_fd != STDIN_FILENO) close(in_fd);
        in_fd = fds[0];
    }

    // last command
    if ((pids[n++] = fork()) == 0)
    {
        dup2(in_fd, STDIN_FILENO);
        if (in_fd != STDIN_FILENO) close(in_fd);
        child_exec_one(c);
    }
    if (in_fd != STDIN_FILENO) close(in_fd);

    // wait all, return the last exit status
    int status = 0;
    for (int i = 0; i < n; i++)
        waitpid(pids[i], &status, 0);

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

// 2½) A little helper for the “no‑pipeline, external” case
static int exec_single(t_command *cmd)
{
    pid_t pid = fork();
    if (pid < 0)      { minishell_perror("fork"); return 1; }
    if (pid == 0)     child_exec_one(cmd);
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

// 4) Top‑level dispatcher (in your parent shell)
int execute_command(t_command *cmd)
{
    int saved_in  = -1, saved_out = -1, status;

    // apply < redirection in the parent
    if (cmd->input_file) {
        saved_in = dup(STDIN_FILENO);
        int in = open(cmd->input_file, O_RDONLY);
        if (in < 0) { minishell_perror(cmd->input_file); return 1; }
        dup2(in, STDIN_FILENO);
        close(in);
    }

    // apply > / >> redirection in the parent
    if (cmd->output_file) {
        saved_out = dup(STDOUT_FILENO);
        int flags = cmd->append_mode
            ? (O_WRONLY|O_CREAT|O_APPEND)
            : (O_WRONLY|O_CREAT|O_TRUNC);
        int out = open(cmd->output_file, flags, 0644);
        if (out < 0) { minishell_perror(cmd->output_file); return 1; }
        dup2(out, STDOUT_FILENO);
        close(out);
    }

    // dispatch
    if (cmd->next)
        status = exec_pipeline(cmd);
    else if (is_builtin(cmd->cmd))
        status = execute_builtin(cmd);
    else
        status = exec_single(cmd);

    // restore stdio
    if (saved_in  >= 0) { dup2(saved_in,  STDIN_FILENO);  close(saved_in); }
    if (saved_out >= 0) { dup2(saved_out, STDOUT_FILENO); close(saved_out); }

    return status;
}
