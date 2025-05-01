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

// child_exec_one: sets up pipes, redirections, builtins, external exec
static void child_exec_one(t_command *cmd, int pipe_in_fd, int pipe_out_fd)
{
    int fd;

    // A) empty‐string => no‐op
    if (!cmd->cmd || cmd->cmd[0] == '\0')
        _exit(0);

    // B) Always wire the pipe ends first
    if (pipe_out_fd >= 0)
    {
        dup2(pipe_out_fd, STDOUT_FILENO);
        close(pipe_out_fd);
    }
    if (pipe_in_fd >= 0)
    {
        dup2(pipe_in_fd, STDIN_FILENO);
        close(pipe_in_fd);
    }

    // C) all '>' / '>>' redirections first
    for (int i = 0; i < cmd->out_count; i++)
    {
        int flags = O_WRONLY | O_CREAT |
                   (cmd->append_flags[i] ? O_APPEND : O_TRUNC);
        fd = open(cmd->out_files[i], flags, 0644);
        if (fd < 0) { minishell_perror(cmd->out_files[i]); _exit(1); }
        if (i == cmd->out_count - 1)
            dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    // D) all '<' redirections next
    for (int i = 0; i < cmd->in_count; i++)
    {
        fd = open(cmd->in_files[i], O_RDONLY);
        if (fd < 0) { minishell_perror(cmd->in_files[i]); _exit(1); }
        if (i == cmd->in_count - 1)
            dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // E) here-doc handling if needed ...

    // F) builtin in a pipeline?
    if (is_builtin(cmd->cmd))
        _exit(execute_builtin(cmd));

    // G) external exec
    char **argv = calloc(cmd->arg_count + 2, sizeof *argv);
    argv[0] = cmd->cmd;
    for (int i = 0; i < cmd->arg_count; i++)
        argv[i+1] = cmd->args[i];
    argv[cmd->arg_count+1] = NULL;

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

    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        write(2, "minishell: ", 11);
        write(2, cmd->cmd, strlen(cmd->cmd));
        write(2, ": Is a directory\n", 17);
        _exit(126);
    }

    execve(path, argv, environ);
    minishell_perror(cmd->cmd);
	if (errno == EACCES || errno == EISDIR)
	_exit(126);
	else
	_exit(127);
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



int exec_pipeline(t_command *head)
{
    int n = 0;
    for (t_command *c = head; c; c = c->next) n++;
    if (n == 0) return 0;

    t_command **st = malloc(n * sizeof *st);
    for (int i = 0; i < n; i++)
    {
        st[i] = head;
        head = head->next;
    }

    int (*pipes)[2] = malloc((n-1) * sizeof pipes[0]);
    for (int i = 0; i < n-1; i++)
        if (pipe(pipes[i]) < 0) { perror("pipe"); exit(1); }

    pid_t *pids = malloc(n * sizeof *pids);
    for (int i = n-1; i >= 0; i--)
    {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(1); }
        if (pid == 0)
        {
            int in_fd  = (i > 0)   ? pipes[i-1][0] : -1;
            int out_fd = (i < n-1) ? pipes[i][1]   : -1;
            for (int j = 0; j < n-1; j++)
            {
                if (pipes[j][0] != in_fd)  close(pipes[j][0]);
                if (pipes[j][1] != out_fd) close(pipes[j][1]);
            }
            child_exec_one(st[i], in_fd, out_fd);
            _exit(1);
        }
        pids[i] = pid;
    }

    for (int i = 0; i < n-1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    int status = 0;
    for (int i = 0; i < n; i++)
        waitpid(pids[i], &status, 0);
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
