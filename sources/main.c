#include "../include/minishell.h"

static void fix_empty_cmd(t_command *c)
{
    if (c->cmd[0] == '\0' && c->arg_count > 0)
    {
        /* 1) free the bogus empty cmd, grab args[0] as the real cmd */
        free(c->cmd);
        c->cmd = strdup(c->args[0]);

        /* 2) slide everything else down one slot */
        for (int i = 1; i < c->arg_count; i++)
            c->args[i-1] = c->args[i];

        /* 3) shrink the args[] array and NULL-terminate it */
        c->arg_count--;
        c->args = realloc(c->args, (c->arg_count + 1) * sizeof *c->args);
        if (!c->args && c->arg_count > 0)
            exit_with_error("realloc");
        c->args[c->arg_count] = NULL;
    }
}
static void init_shlvl(void) {
    char *old = getenv("SHLVL");
    int lvl = old ? atoi(old) + 1 : 1;
    char buf[16];
    snprintf(buf, sizeof buf, "%d", lvl);
    // update the environment; 1 == overwrite existing
    setenv("SHLVL", buf, 1);
}

static void	do_expansion(t_command* cmds, int last_status)
{
	t_command* c = cmds;
	while (c)
	{
		expand_command_arguments(c, last_status);
		fix_empty_cmd(c);
		c = c->next;
	}
}

// The main read–eval–print loop
static void	shell_loop(void)
{
	char* input;
	t_token* tokens;
	t_command* cmds;
	int			parse_status;
	int			last_exit = 0;

	while (1)
	{
		input = readline(PROMPT);

		// pick up SIGINT (130)
		if (g_last_exit_status)
		{
			last_exit = g_last_exit_status;
			g_last_exit_status = 0;
		}

		if (!input)
		{
			printf("exit\n");
			break;
		}

		if (*input)
			add_history(input);

		tokens = tokenize(input);
		parse_status = parse_pipeline(tokens, &cmds);
		free_tokens(tokens);

		if (parse_status == 0 && cmds)
		{
			do_expansion(cmds, last_exit);
			last_exit = execute_command(cmds);
			free_command(cmds);
		}
		else
		{
			last_exit = 2;
		}

		free(input);
	}
}

int	main(void)
{
	init_signals();
	init_shlvl();
	shell_loop();
	return g_last_exit_status;
}
