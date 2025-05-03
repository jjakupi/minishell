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

static void shell_loop(t_shell *shell) //edited
{
	char		*input;
	t_token		*tokens;
	t_command	*cmds;
	int			parse_status;

	while (1)
	{
		input = readline(PROMPT);
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
			add_history(input);
		tokens = tokenize(input);
		parse_status = parse_pipeline(tokens, &cmds);
		free_tokens(tokens);
		if (parse_status == 0 && cmds)
		{
			do_expansion(cmds, shell->last_exit);
			shell->last_exit = execute_command(cmds, shell);
			free_command(cmds);
		}
		else
			shell->last_exit = 2;
		free(input);
	}
}

int	main(int argc, char **argv, char **envp) // edited
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_signals();
	shell.envp = envp;
	shell.last_exit = 0;
	shell_loop(&shell);
	return (shell.last_exit);
}
