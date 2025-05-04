/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:19:41 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 19:38:42 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command	**build_stage_array(t_command *head, int n)
{
	t_command	**arr;
	int			i;

	arr = malloc(n * sizeof(*arr));
	if (arr == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < n)
	{
		arr[i] = head;
		head = head->next;
		i++;
	}
	return (arr);
}

void	make_pipes(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

int	wait_for_children(pid_t *pids, int n)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}

void	close_unused_pipes(int (*pipes)[2], int count, int in_fd, int out_fd)
{
	int	j;

	j = 0;
	while (j < count)
	{
		if (pipes[j][0] != in_fd)
		safe_close(pipes[j][0]);
		if (pipes[j][1] != out_fd)
			safe_close(pipes[j][1]);
		j++;
	}
}

void	close_all_pipes(int (*pipes)[2], int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		safe_close(pipes[j][0]);
		safe_close(pipes[j][1]);
		j++;
	}
}
