/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:47:36 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 14:12:51 by julrusse         ###   ########.fr       */
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
		close(fd);
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
				close(saved_out);
			}
			return (1);
		}
		*saved_in = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
		i++;
	}
	return (0);
}

static void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in >= 0)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out >= 0)
	{
		dup2(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
}

static int	perform_builtin_with_redirs(t_command *cmd)
{
	int	saved_in;
	int	saved_out;
	int	status;

	if (apply_output_redirs(cmd, &saved_out) != 0)
		return (1);
	if (apply_input_redirs(cmd, saved_out, &saved_in) != 0)
		return (1);
	status = execute_builtin(cmd);
	restore_stdio(saved_in, saved_out);
	return (status);
}

int	execute_command(t_command *cmd)
{
	int	status;
	int	pipeline;
	int	standalone_builtin;

	if (cmd == NULL || cmd->cmd == NULL || cmd->cmd[0] == '\0')
		return (0);
	pipeline = (cmd->next != NULL);
	standalone_builtin = (!pipeline && is_builtin(cmd->cmd));
	if (standalone_builtin)
		return (perform_builtin_with_redirs(cmd));
	if (pipeline)
		status = exec_pipeline(cmd);
	else
		status = exec_single(cmd);
	return (status);
}
/*
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
                if (saved_out >= 0) { dup2(saved_out, STDOUT_FILENO);
					close(saved_out); }
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
*/
