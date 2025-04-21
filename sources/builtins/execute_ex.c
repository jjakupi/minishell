#include "../include/minishell.h"

extern char **environ;

// 1) Find an executable on $PATH
static char *find_executable(const char *name)
{
    // if it contains a slash, treat it as a path
    if (strchr(name, '/')) {
        if (access(name, X_OK) == 0)
            return strdup(name);
        else
            return NULL;
    }

    // otherwise search each directory in PATH
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

// 2) In a child, set up heredoc & file redirections, then exec one command
static void child_exec_one(t_command *cmd)
{
    // -- heredoc first --
    if (cmd->has_heredoc)
    {
        int hpipe[2];
        pipe(hpipe);
        for (;;)
        {
            char *line = readline("> ");
            if (!line || strcmp(line, cmd->heredoc_delimiter) == 0)
            {
                free(line);
                break;
            }
            write(hpipe[1], line, strlen(line));
            write(hpipe[1], "\n", 1);
            free(line);
        }
        close(hpipe[1]);
        dup2(hpipe[0], STDIN_FILENO);
        close(hpipe[0]);
    }

    // -- file redirections --
    if (cmd->input_file)
    {
        int in = open(cmd->input_file, O_RDONLY);
        if (in < 0) { perror(cmd->input_file); exit(1); }
        dup2(in, STDIN_FILENO);
        close(in);
    }
    if (cmd->output_file)
    {
        int flags = cmd->append_mode
            ? (O_WRONLY | O_CREAT | O_APPEND)
            : (O_WRONLY | O_CREAT | O_TRUNC);
        int out = open(cmd->output_file, flags, 0644);
        if (out < 0) { perror(cmd->output_file); exit(1); }
        dup2(out, STDOUT_FILENO);
        close(out);
    }

    // -- built‑in? (only if it’s a standalone, no next) --
    if (is_builtin(cmd->cmd) && !cmd->next)
        exit(execute_builtin(cmd));

    // -- external: build argv and execve --
    char **argv = calloc(cmd->arg_count + 2, sizeof(char *));
    argv[0] = cmd->cmd;
    for (int i = 0; i < cmd->arg_count; i++)
        argv[i+1] = cmd->args[i];

    char *path = find_executable(cmd->cmd);
    if (!path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->cmd);
        exit(127);
    }
    execve(path, argv, environ);
    perror("execve");
    exit(126);
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
    if (pid < 0)      { perror("fork"); return 1; }
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
        if (in < 0) { perror(cmd->input_file); return 1; }
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
        if (out < 0) { perror(cmd->output_file); return 1; }
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
