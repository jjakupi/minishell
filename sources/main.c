#include "../include/minishell.h"

static void	do_expansion(t_command *cmds, int last_status)
{
	t_command	*c;

	c = cmds;
	while (c)
	{
		expand_command_arguments(c, last_status);
		fix_empty_cmd(c);
		c = c->next;
	}
}

static void	shell_loop(void)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmds;
	int			parse_status;
	int			last_exit;

	last_exit = 0;
	while (1)
	{
		input = readline(PROMPT);
		if (g_last_exit_status)
		{
			last_exit = g_last_exit_status;
			g_last_exit_status = 0;
		}
		if (!input)
		{
			printf ("exit\n");
			break;
		}
		if (input[0] == '\0')
		{
			free (input);
			continue;
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
			last_exit = 2;
		free(input);
	}
}

int	main(void)
{
	init_signals();
	shell_loop();
	return (g_last_exit_status);
}
