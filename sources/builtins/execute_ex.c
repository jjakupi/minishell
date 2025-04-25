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

// 1) Find an executable on $PATH
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

// child_exec_one: sets up redirections, builtins, external exec
static void child_exec_one(t_command *cmd)
{
    // empty command => succeed without doing anything
    if (cmd->cmd[0] == '\0')
        _exit(0);

    // heredoc handling
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

    // input redirection
    if (cmd->input_file)
    {
        int in = open(cmd->input_file, O_RDONLY);
        if (in < 0) { minishell_perror(cmd->input_file); _exit(1); }
        dup2(in, STDIN_FILENO);
        close(in);
    }
    // output/append redirection
    if (cmd->output_file)
    {
        int flags = cmd->append_mode
            ? (O_WRONLY | O_CREAT | O_APPEND)
            : (O_WRONLY | O_CREAT | O_TRUNC);
        int out = open(cmd->output_file, flags, 0644);
        if (out < 0) { minishell_perror(cmd->output_file); _exit(1); }
        dup2(out, STDOUT_FILENO);
        close(out);
    }

    // builtin inline if standalone (no pipeline)
    if (is_builtin(cmd->cmd) && !cmd->next)
        _exit(execute_builtin(cmd));

    // build argv
    char **argv = calloc(cmd->arg_count + 2, sizeof *argv);
    argv[0] = cmd->cmd;
    for (int i = 0; i < cmd->arg_count; i++)
        argv[i+1] = cmd->args[i];
    argv[cmd->arg_count+1] = NULL;

    // determine exec path
    char *path;
    if (strchr(cmd->cmd, '/'))
    {
        path = cmd->cmd;
        if (access(path, F_OK) != 0)
        {
            minishell_perror(path);
            _exit(127);
        }
    }
    else
    {
        path = find_executable(cmd->cmd);
        if (!path)
        {
            write(2, "minishell: ", 11);
            write(2, cmd->cmd, strlen(cmd->cmd));
            write(2, ": command not found\n", 20);
            _exit(127);
        }
    }

    // directory check
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        write(2, "minishell: ", 11);
        write(2, cmd->cmd, strlen(cmd->cmd));
        write(2, ": Is a directory\n", 17);
        _exit(126);
    }

    // exec
    execve(path, argv, environ);
    minishell_perror(cmd->cmd);
    if (errno == EACCES || errno == EISDIR)
        _exit(126);
    else
        _exit(127);
}

// exec_pipeline: forks children for each pipeline stage
int exec_pipeline(t_command *head)
{
    t_command *c = head;
    int in_fd = STDIN_FILENO;
    pid_t pids[64];
    int n = 0;

    for (; c->next; c = c->next)
    {
        int fds[2];
        pipe(fds);
        pids[n] = fork();
        if (pids[n] == 0)
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
        n++;
    }
    // last stage
    pids[n] = fork();
    if (pids[n] == 0)
    {
        dup2(in_fd, STDIN_FILENO);
        if (in_fd != STDIN_FILENO) close(in_fd);
        child_exec_one(c);
    }
    if (in_fd != STDIN_FILENO) close(in_fd);
    n++;

    int status = 0;
    for (int i = 0; i < n; i++)
        waitpid(pids[i], &status, 0);

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

// exec_single: for a lone external command
static int exec_single(t_command *cmd)
{
    pid_t pid = fork();
    if (pid < 0)  { minishell_perror("fork"); return 1; }
    if (pid == 0) child_exec_one(cmd);
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

// execute_command: dispatch, but only the parent handles standalone builtins’ redirects
int execute_command(t_command *cmd)
{
    int saved_in = -1, saved_out = -1;
    int status;
    bool pipeline           = (cmd->next != NULL);
    bool standalone_builtin = (!pipeline && is_builtin(cmd->cmd));

    if (standalone_builtin)
    {
        if (cmd->input_file)
        {
            int in = open(cmd->input_file, O_RDONLY);
            if (in < 0) return (minishell_perror(cmd->input_file), 1);
            saved_in = dup(STDIN_FILENO);
            dup2(in, STDIN_FILENO);
            close(in);
        }
        if (cmd->output_file)
        {
            int flags = cmd->append_mode
                      ? (O_WRONLY|O_CREAT|O_APPEND)
                      : (O_WRONLY|O_CREAT|O_TRUNC);
            int out = open(cmd->output_file, flags, 0644);
            if (out < 0) return (minishell_perror(cmd->output_file), 1);
            saved_out = dup(STDOUT_FILENO);
            dup2(out, STDOUT_FILENO);
            close(out);
        }
        status = execute_builtin(cmd);
        if (saved_in  >= 0) { dup2(saved_in,  STDIN_FILENO);  close(saved_in);  }
        if (saved_out >= 0) { dup2(saved_out, STDOUT_FILENO); close(saved_out); }
        return status;
    }

    if (pipeline)
        status = exec_pipeline(cmd);
    else if (is_builtin(cmd->cmd))
        status = exec_single(cmd);  // fork for builtins in pipelines or alone
    else
        status = exec_single(cmd);

    return status;
}
