#include "../include/minishell.h"

// Fork each stage in reverse order, wiring pipes and launching
static void spawn_pipeline(t_command **st, int (*pipes)[2], pid_t *pids, int n)
{
	int i = n;
	while (--i >= 0)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			int in_fd  = i > 0       ? pipes[i - 1][0] : -1;
			int out_fd = i < n - 1   ? pipes[i][1]     : -1;
			close_unused_pipes(pipes, n - 1, in_fd, out_fd);
			child_exec_one(st[i], in_fd, out_fd);
			_exit(1);
		}
		pids[i] = pid;
	}
}

int exec_pipeline(t_command *head)
{
	int n = count_stages(head);
	if (n == 0)
		return 0;

	t_command **st = build_stage_array(head, n);
	int (*pipes)[2] = malloc((n - 1) * sizeof *pipes);
	make_pipes(pipes, n - 1);
	pid_t *pids = malloc(n * sizeof *pids);

	spawn_pipeline(st, pipes, pids, n);

	close_all_pipes(pipes, n - 1);
	int code = wait_for_children(pids, n);

	free(st);
	free(pipes);
	free(pids);
	return code;
}
