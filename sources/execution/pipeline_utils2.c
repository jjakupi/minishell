/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:21:10 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 14:30:19 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_stages(t_command *head)
{
	int	n;

	n = 0;
	while (head)
	{
		n++;
		head = head->next;
	}
	return (n);
}

void	pipeline_child(t_command *cmd, int (*pipes)[2], int idx, int total)
{
	int	in_fd;
	int	out_fd;

	if (idx > 0)
		in_fd = pipes[idx - 1][0];
	else
		in_fd = -1;
	if (idx < total - 1)
		out_fd = pipes[idx][1];
	else
		out_fd = -1;
	close_unused_pipes(pipes, total - 1, in_fd, out_fd);
	child_exec_one(cmd, in_fd, out_fd);
	_exit(1);
}
