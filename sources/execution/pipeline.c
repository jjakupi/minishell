/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:21:10 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 14:37:02 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	spawn_pipeline(t_command **st, int (*pipes)[2], pid_t *pids, int n)
{
	int		i;
	pid_t	pid;

	i = n - 1;
	while (i >= 0)
	{
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
			pipeline_child(st[i], pipes, i, n);
		pids[i] = pid;
		i--;
	}
}

static int	alloc_pipeline_resources(t_command *head,
		t_command ***stp, int (**pipes)[2], pid_t **pidsp)
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

static void	free_pipeline_resources(t_command **st, int (*pipes)[2],
	pid_t *pids)
{
	free(st);
	free(pipes);
	free(pids);
}

int	exec_pipeline(t_command *head)
{
	t_command	**st;
	int			(*pipes)[2];
	pid_t		*pids;
	int			status;
	int			n;

	n = alloc_pipeline_resources(head, &st, &pipes, &pids);
	if (n < 0)
		return (1);
	if (n == 0)
		return (0);
	make_pipes(pipes, n - 1);
	spawn_pipeline(st, pipes, pids, n);
	close_all_pipes(pipes, n - 1);
	status = wait_for_children(pids, n);
	free_pipeline_resources(st, pipes, pids);
	return (status);
}
