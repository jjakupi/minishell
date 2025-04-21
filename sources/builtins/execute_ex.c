#include "../include/minishell.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern char **environ;
// 1) Find the full path of the command by scanning $PATH with stat()
static char *find_executable(const char *name) {
    char *path_env = getenv("PATH");
    if (!path_env) return NULL;
    char *paths = strdup(path_env);
    char *dir = strtok(paths, ":");
    while (dir) {
        char candidate[1024];
        snprintf(candidate, sizeof(candidate), "%s/%s", dir, name);
        struct stat st;
        if (stat(candidate, &st) == 0 && (st.st_mode & S_IXUSR)) {
            free(paths);
            return strdup(candidate);
        }
        dir = strtok(NULL, ":");
    }
    free(paths);
    return NULL;
}

// 2) Execute a single command (no pipeline).  Returns its exit status.
int exec_single(t_command *cmd) {
    char *exe = find_executable(cmd->cmd);
    if (!exe) {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->cmd);
        return 127;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(exe);
        return 1;
    }
    if (pid == 0) {
        // Child: set up redirections if any
        if (cmd->input_file) {
            int fd = open(cmd->input_file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (cmd->output_file) {
            int flags = cmd->append_mode ? (O_WRONLY|O_CREAT|O_APPEND)
                                        : (O_WRONLY|O_CREAT|O_TRUNC);
            int fd = open(cmd->output_file, flags, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Build argv array
        char **argv = malloc((cmd->arg_count+2)*sizeof(char*));
        argv[0] = exe;
        for (int i = 0; i < cmd->arg_count; i++)
            argv[i+1] = cmd->args[i];
        argv[cmd->arg_count+1] = NULL;

        execve(exe, argv, environ);
        // if execve returns, it failed:
        perror("execve");
        _exit(127);
    }

    // Parent: wait for child
    int status = 0;
    waitpid(pid, &status, 0);
    free(exe);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

// 3) For a pipeline of N > 1 commands, you loop and chain pipes:
int exec_pipeline(t_command *head) {
    int in_fd = STDIN_FILENO;
    for (t_command *c = head; c->next; c = c->next) {
        int fds[2];
        pipe(fds);
        pid_t pid = fork();
        if (pid == 0) {
            // child: read from in_fd, write to fds[1]
            dup2(in_fd, STDIN_FILENO);
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]); close(fds[1]);
            // do any infile/outfile redirs on c …
            exec_single(c);
            _exit(1);
        }
        // parent: close write‐end, carry read‐end forward
        close(fds[1]);
        if (in_fd != STDIN_FILENO) close(in_fd);
        in_fd = fds[0];
    }
    // last command reads from in_fd, writes to STDOUT
    if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    return exec_single(head);  // runs the final c->next==NULL
}

int execute_command(t_command *cmds)
{
    // built‑in?
    if (is_builtin(cmds->cmd))
        return execute_builtin(cmds);

    // external
    // here we only handle the “single command, no pipeline” case;
    // for pipelines you’d detect cmds->next != NULL and fork multiple times
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        // child: do file‑redirections first
        if (cmds->input_file) {
            int in = open(cmds->input_file, O_RDONLY);
            if (in < 0) { perror(cmds->input_file); exit(1); }
            dup2(in, STDIN_FILENO);
            close(in);
        }
        if (cmds->output_file) {
            int flags = cmds->append_mode
                ? (O_WRONLY|O_CREAT|O_APPEND)
                : (O_WRONLY|O_CREAT|O_TRUNC);
            int out = open(cmds->output_file, flags, 0644);
            if (out < 0) { perror(cmds->output_file); exit(1); }
            dup2(out, STDOUT_FILENO);
            close(out);
        }

        // build argv[]
        char **argv = malloc((cmds->arg_count+2)*sizeof(char*));
        argv[0] = cmds->cmd;
        for (int i = 0; i < cmds->arg_count; i++)
            argv[i+1] = cmds->args[i];
        argv[cmds->arg_count+1] = NULL;

        // find it on $PATH
        char *path = find_executable(cmds->cmd);
        if (!path) {
            fprintf(stderr, "minishell: %s: command not found\n", cmds->cmd);
            exit(127);
        }
        execve(path, argv, environ);
        perror("execve");
        exit(126);
    }

    // parent: wait
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
