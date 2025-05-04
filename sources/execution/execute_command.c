/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:47:36 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 19:38:33 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	apply_output_redirs(t_command *cmd, int *saved_out)
{
	int	i;
	int	fd;
	int	flags;

	i = 0;
	*saved_out = -1;
	while (i < cmd->out_count)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append_flags[i])
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(cmd->out_files[i], flags, 0644);
		if (fd < 0)
		{
			minishell_perror(cmd->out_files[i]);
			return (1);
		}
		*saved_out = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		safe_close(fd);
		i++;
	}
	return (0);
}

static int	apply_input_redirs(t_command *cmd, int saved_out, int *saved_in)
{
	int	i;
	int	fd;

	i = 0;
	*saved_in = -1;
	while (i < cmd->in_count)
	{
		fd = open(cmd->in_files[i], O_RDONLY);
		if (fd < 0)
		{
			minishell_perror(cmd->in_files[i]);
			if (saved_out >= 0)
			{
				dup2(saved_out, STDOUT_FILENO);
				safe_close(saved_out);
			}
			return (1);
		}
		*saved_in = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		safe_close(fd);
		i++;
	}
	return (0);
}

static void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in >= 0)
	{
		dup2(saved_in, STDIN_FILENO);
		safe_close(saved_in);
	}
	if (saved_out >= 0)
	{
		dup2(saved_out, STDOUT_FILENO);
		safe_close(saved_out);
	}
}

static int	perform_builtin_with_redirs(t_command *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;
	int	status;

	if (apply_output_redirs(cmd, &saved_out) != 0)
		return (1);
	if (apply_input_redirs(cmd, saved_out, &saved_in) != 0)
		return (1);
	status = execute_builtin(cmd, shell);
	restore_stdio(saved_in, saved_out);
	return (status);
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	int	status;
	int	pipeline;
	int	standalone_builtin;

	if (cmd == NULL || cmd->cmd == NULL || cmd->cmd[0] == '\0')
		return (0);
	pipeline = (cmd->next != NULL);
	standalone_builtin = (!pipeline && is_builtin(cmd->cmd));
	if (standalone_builtin)
	{
		if (strcmp(cmd->cmd, "exit") == 0)
			write(1, "exit\n", 5);
		return (perform_builtin_with_redirs(cmd, shell));
	}
	if (pipeline)
		status = exec_pipeline(cmd, shell);
	else
		status = exec_single(cmd, shell);
	return (status);
}
