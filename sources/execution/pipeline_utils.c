#include "../include/minishell.h"

// count how many commands are in the pipeline
int count_stages(t_command *head)
{
	int n = 0;
	while (head)
	{
		n++;
		head = head->next;
	}
	return n;
}

// build an array of t_command pointers for indexed access
// returns allocated t_command** of length n

t_command **build_stage_array(t_command *head, int n)
{
	t_command **arr = malloc(n * sizeof *arr);
	if (!arr) { perror("malloc"); exit(1); }
	int i = 0;
	while (i < n)
	{
		arr[i] = head;
		head = head->next;
		i++;
	}
	return arr;
}

// create n pipes (stored in pipes[0..n-1])
void make_pipes(int (*pipes)[2], int n)
{
	int i = 0;
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

// wait for all child pids, return the last exit code
int wait_for_children(pid_t *pids, int n)
{
	int status = 0;
	int i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	else
		return 1;
}

// close all pipe fds except the designated in_fd and out_fd
void close_unused_pipes(int (*pipes)[2], int count, int in_fd, int out_fd)
{
	int j = 0;
	while (j < count)
	{
		if (pipes[j][0] != in_fd)
			close(pipes[j][0]);
		if (pipes[j][1] != out_fd)
			close(pipes[j][1]);
		j++;
	}
}

// close every pipe end
void close_all_pipes(int (*pipes)[2], int count)
{
	int j = 0;
	while (j < count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}
