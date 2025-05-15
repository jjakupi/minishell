/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:32:21 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 15:41:31 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_redirect_heredoc(int read_end)
{
	dup2(read_end, STDIN_FILENO);
	safe_close(read_end);
}

void	handle_empty(t_command *cmd)
{
	if (!cmd->cmd || cmd->cmd[0] == '\0')
		_exit(0);
}

void	wire_pipes(int in_fd, int out_fd)
{
	if (out_fd >= 0)
	{
		dup2(out_fd, STDOUT_FILENO);
		safe_close(out_fd);
	}
	if (in_fd >= 0)
	{
		dup2(in_fd, STDIN_FILENO);
		safe_close(in_fd);
	}
}

void	apply_output_redirects(t_command *cmd)
{
	int	i;
	int	flags;
	int	fd;

	i = 0;
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
			_exit(1);
		}
		if (i == cmd->out_count - 1)
			dup2(fd, STDOUT_FILENO);
		safe_close(fd);
		i++;
	}
}

void	apply_input_redirects(t_command *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->in_count)
	{
		fd = open(cmd->in_files[i], O_RDONLY);
		if (fd < 0)
		{
			minishell_perror(cmd->in_files[i]);
			_exit(1);
		}
		if (i == cmd->in_count - 1)
			dup2(fd, STDIN_FILENO);
		safe_close(fd);
		i++;
	}
}
