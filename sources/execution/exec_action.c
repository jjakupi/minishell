#include "../include/minishell.h"

extern char **environ;

// F) Builtin execution in a pipeline
void	run_builtin_or_exit(t_command *cmd)
{
	if (is_builtin(cmd->cmd))
		_exit(execute_builtin(cmd));
}

// G1) Build argv array
char **build_argv(t_command *cmd)
{
	char **argv = calloc(cmd->arg_count + 2, sizeof(*argv));
	argv[0] = cmd->cmd;
	int i = 0;
	while (i < cmd->arg_count) {
		argv[i + 1] = cmd->args[i];
		i++;
	}
	argv[cmd->arg_count + 1] = NULL;
	return argv;
}

// G2) Resolve path or error
char	*resolve_path(const char *name)
{
	if (strchr(name, '/') != NULL) {
		if (access(name, F_OK) != 0) {
			minishell_perror(name);
			_exit(127);
		}
		return (char *)name;
	}
	char *path = find_executable(name);
	if (!path) {
		write(2, "minishell: ", 11);
		write(2, name, strlen(name));
		write(2, ": command not found\n", 20);
		_exit(127);
	}
	return path;
}

// G3) Check directory and exec
void	exec_external(t_command *cmd)
{
	char **argv = build_argv(cmd);
	char *path  = resolve_path(cmd->cmd);
	struct stat st;
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
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

// H) Orchestrator: child entry point
void		child_exec_one(t_command *cmd, int in_fd, int out_fd)
{
	handle_empty(cmd);
	wire_pipes(in_fd, out_fd);
	apply_output_redirects(cmd);
	apply_input_redirects(cmd);
	apply_heredoc(cmd);
	run_builtin_or_exit(cmd);
	exec_external(cmd);
}
