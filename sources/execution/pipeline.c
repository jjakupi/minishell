/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:21:10 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 19:38:50 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	spawn_pipeline(t_command **st, int (*pipes)[2], pid_t *pids,
			int n, t_shell *shell)
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
			pipeline_child(st[i], pipes, i, n, shell);
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

int exec_pipeline(t_command *head, t_shell *shell)
{
    t_command **st;
    int (*pipes)[2];
    pid_t *pids;
    int    n, status;
    int    old_stdin;

    n = alloc_pipeline_resources(head, &st, &pipes, &pids);
    if (n < 0) return 1;
    if (n == 0) return 0;

    make_pipes(pipes, n - 1);

    // 1) If the *first* stage has a here-doc, do the same hijack trick:
    if (head->has_heredoc)
    {
        int hp[2];
        old_stdin = dup(STDIN_FILENO);
        if (pipe(hp) < 0)
            exit_with_error("pipe");
        read_heredoc_lines(head->heredoc_delimiter, hp[1]);
        safe_close(hp[1]);
        dup2(hp[0], STDIN_FILENO);
        safe_close(hp[0]);
    }

    // 2) Now spawn the pipeline children; they inherit stdin→heredoc
    spawn_pipeline(st, pipes, pids, n, shell);

    // 3) Parent restores its real stdin immediately
    if (head->has_heredoc)
    {
        dup2(old_stdin, STDIN_FILENO);
        safe_close(old_stdin);
    }

    close_all_pipes(pipes, n - 1);
    status = wait_for_children(pids, n);
    free_pipeline_resources(st, pipes, pids);
    return status;
}

