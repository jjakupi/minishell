#include "../include/minishell.h"

void	handle_empty(t_command *cmd)
{
	if (!cmd->cmd || cmd->cmd[0] == '\0')
		_exit(0);
}

// B) Pipe wiring
void	wire_pipes(int in_fd, int out_fd)
{
	if (out_fd >= 0) {
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (in_fd >= 0) {
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}

// C) Output redirections (`>` / `>>`)
void	apply_output_redirects(t_command *cmd)
{
	int	i = 0;
	while (i < cmd->out_count) {
		int flags = O_WRONLY | O_CREAT;
		if (cmd->append_flags[i])
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		int fd = open(cmd->out_files[i], flags, 0644);
		if (fd < 0) {
			minishell_perror(cmd->out_files[i]);
			_exit(1);
		}
		if (i == cmd->out_count - 1)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}

// D) Input redirections (`<`)
void	apply_input_redirects(t_command *cmd)
{
	int	i = 0;
	while (i < cmd->in_count) {
		int fd = open(cmd->in_files[i], O_RDONLY);
		if (fd < 0) {
			minishell_perror(cmd->in_files[i]);
			_exit(1);
		}
		if (i == cmd->in_count - 1)
			dup2(fd, STDIN_FILENO);
		close(fd);
		i++;
	}
}

// E) Heredoc handling
void	apply_heredoc(t_command *cmd)
{
	if (!cmd->has_heredoc)
		return;
	int	hp[2];
	if (pipe(hp) < 0) {
		minishell_perror("pipe");
		_exit(1);
	}
	for (;;) {
		char *line = readline("> ");
		if (!line || strcmp(line, cmd->heredoc_delimiter) == 0) {
			free(line);
			break;
		}
		write(hp[1], line, strlen(line));
		write(hp[1], "\n", 1);
		free(line);
	}
	close(hp[1]);
	dup2(hp[0], STDIN_FILENO);
	close(hp[0]);
}
