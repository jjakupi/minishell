/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:21:10 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 15:45:59 by julrusse         ###   ########.fr       */
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

static int	pipeline_setup(t_command *head, t_shell *shell, t_command ***stp,
				pid_t **pidsp)
{
	int	(*pipes)[2];
	int	n;

	n = alloc_pipeline_resources(head, stp, &pipes, pidsp);
	if (n <= 0)
		return (n);
	shell->pipes = pipes;
	shell->pipe_count = n - 1;
	make_pipes(pipes, n - 1);
	return (n);
}

static int	pipeline_heredoc_in(t_command *head, int *old_stdin)
{
	int	hp[2];

	*old_stdin = -1;
	if (!head->has_heredoc)
		return (0);
	*old_stdin = dup(STDIN_FILENO);
	if (*old_stdin < 0)
		return (minishell_perror("dup"), 1);
	if (pipe(hp) < 0)
		exit_with_error("pipe");
	read_heredoc_lines(head->heredoc_delimiter, hp[1]);
	safe_close(hp[1]);
	dup2(hp[0], STDIN_FILENO);
	safe_close(hp[0]);
	return (0);
}

static void	pipeline_heredoc_out(int old_stdin)
{
	if (old_stdin >= 0)
	{
		dup2(old_stdin, STDIN_FILENO);
		safe_close(old_stdin);
	}
}

int	exec_pipeline(t_command *head, t_shell *shell)
{
	t_command	**st;
	pid_t		*pids;
	int			n;
	int			status;
	int			old_stdin;

	n = pipeline_setup(head, shell, &st, &pids);
	if (n < 0)
		return (1);
	if (n == 0)
		return (0);
	if (pipeline_heredoc_in(head, &old_stdin))
		return (1);
	spawn_pipeline(st, pids, shell);
	pipeline_heredoc_out(old_stdin);
	close_all_pipes(shell->pipes, shell->pipe_count);
	status = wait_for_children(pids, n);
	free_pipeline_resources(st, shell->pipes, pids);
	return (status);
}
