/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:47:36 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/16 09:48:09 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	open_and_dup_output(char *file, int append)
{
	int		flags;
	int		fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		minishell_perror(file);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	safe_close(fd);
	return (0);
}

static int	apply_output_redirs(t_command *cmd, int *saved_out)
{
	int		i;

	*saved_out = -1;
	if (cmd->out_count == 0)
		return (0);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_out < 0)
	{
		perror("dup");
		return (1);
	}
	i = 0;
	while (i < cmd->out_count)
	{
		if (open_and_dup_output(cmd->out_files[i],
				cmd->append_flags[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}

static int	apply_input_redirs(t_command *cmd, int *saved_in)
{
	int	fd;
	int	i;

	*saved_in = -1;
	if (cmd->in_count <= 0)
		return (0);
	*saved_in = dup(STDIN_FILENO);
	if (*saved_in < 0)
		return (perror("dup"), 1);
	i = 0;
	while (i < cmd->in_count)
	{
		fd = open(cmd->in_files[i], O_RDONLY);
		if (fd < 0)
		{
			minishell_perror(cmd->in_files[i]);
			return (1);
		}
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
	if (apply_input_redirs(cmd, &saved_in) != 0)
	{
		restore_stdio(-1, saved_out);
		return (1);
	}
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
		if (ft_strcmp(cmd->cmd, "exit") == 0)
			write(STDOUT_FILENO, "exit\n", 5);
		return (perform_builtin_with_redirs(cmd, shell));
	}
	if (pipeline)
		status = exec_pipeline(cmd, shell);
	else
		status = exec_single(cmd, shell);
	return (status);
}
