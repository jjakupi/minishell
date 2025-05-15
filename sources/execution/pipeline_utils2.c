/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:21:10 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 15:51:46 by julrusse         ###   ########.fr       */
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

int	alloc_pipeline_resources(t_command *head, t_command ***stp,
		int (**pipes)[2], pid_t **pidsp)
{
	int	n;

	n = count_stages(head);
	if (n <= 0)
		return (n);
	*stp = build_stage_array(head, n);
	*pipes = malloc((n - 1) * sizeof(**pipes));
	if (*pipes == NULL)
		return (-1);
	*pidsp = malloc(n * sizeof(**pidsp));
	if (*pidsp == NULL)
	{
		free(*pipes);
		return (-1);
	}
	return (n);
}

void	pipeline_child(t_command *cmd, int in_fd, int out_fd, t_shell *shell)
{
	close_unused_pipes(shell->pipes, shell->pipe_count, in_fd, out_fd);
	child_exec_one(cmd, in_fd, out_fd, shell);
	_exit(1);
}

static void	spawn_one_stage(int idx, t_command **st, pid_t *pids,
				t_shell *shell)
{
	pid_t	pid;
	int		in_fd;
	int		out_fd;
	int		n;

	n = shell->pipe_count + 1;
	if (idx > 0)
		in_fd = shell->pipes[idx - 1][0];
	else
		in_fd = -1;
	if (idx < n - 1)
		out_fd = shell->pipes[idx][1];
	else
		out_fd = -1;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
		pipeline_child(st[idx], in_fd, out_fd, shell);
	pids[idx] = pid;
}

void	spawn_pipeline(t_command **st, pid_t *pids, t_shell *shell)
{
	int	i;
	int	n;

	n = shell->pipe_count + 1;
	i = n - 1;
	while (i >= 0)
	{
		spawn_one_stage(i, st, pids, shell);
		i--;
	}
}
