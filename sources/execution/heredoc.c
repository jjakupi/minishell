/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:32:21 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 13:42:16 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	open_heredoc_pipe(int hp[2])
{
	if (pipe(hp) < 0)
	{
		minishell_perror("pipe");
		return (-1);
	}
	return (0);
}

void	read_heredoc_lines(const char *delim, int write_end)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(write_end, line, ft_strlen(line));
		write(write_end, "\n", 1);
		free(line);
	}
}

void	redirect_heredoc_input(int hp[2])
{
	close(hp[1]);
	dup2(hp[0], STDIN_FILENO);
	close(hp[0]);
}

void	apply_heredoc(t_command *cmd)
{
	int	hp[2];

	if (cmd == NULL || !cmd->has_heredoc)
		return ;
	if (open_heredoc_pipe(hp) < 0)
		_exit(1);
	read_heredoc_lines(cmd->heredoc_delimiter, hp[1]);
	redirect_heredoc_input(hp);
}
