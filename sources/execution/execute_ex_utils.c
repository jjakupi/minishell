/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ex_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:47:36 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 14:44:55 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static pid_t	fork_and_exec(t_command *cmd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		minishell_perror("fork");
		return ((pid_t) - 1);
	}
	if (pid == 0)
		child_exec_one(cmd, -1, -1, shell);
	return (pid);
}

static int	wait_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	return (status);
}

static int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static void	restore_stdin(int old_stdin)
{
	if (old_stdin >= 0)
	{
		dup2(old_stdin, STDIN_FILENO);
		safe_close(old_stdin);
	}
}

int	exec_single(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		exit_code;
	int		old_stdin;

	old_stdin = -1;
	if (cmd->has_heredoc && handle_heredoc_redir(cmd, &old_stdin))
		return (1);
	pid = fork_and_exec(cmd, shell);
	if (pid < 0)
		return (1);
	restore_stdin(old_stdin);
	status = wait_child(pid);
	exit_code = get_exit_code(status);
	return (exit_code);
}
