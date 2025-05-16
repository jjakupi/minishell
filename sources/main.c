/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:03:03 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/16 11:33:04 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	do_expansion(t_command *cmds, int last_status, char **envp)
{
	t_command	*c;

	c = cmds;
	while (c)
	{
		expand_command_arguments(c, last_status, envp);
		fix_empty_cmd(c);
		c = c->next;
	}
}

static void	handle_line(t_shell *shell, char *input)
{
	t_token		*tokens;
	t_command	*cmds;
	int			status;

	if (*input)
		add_history(input);
	tokens = tokenize(input);
	status = parse_pipeline(tokens, &cmds);
	free_tokens(tokens);
	if (!status && cmds)
	{
		do_expansion(cmds, shell->last_exit, shell->envp);
		shell->last_exit = execute_command(cmds, shell);
		free_command(cmds);
		return ;
	}
	if (g_last_exit_status)
	{
		shell->last_exit = g_last_exit_status;
		g_last_exit_status = 0;
	}
	else
		shell->last_exit = 2;
}

static void	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline(PROMPT);
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (g_last_exit_status)
		{
			shell->last_exit = g_last_exit_status;
			g_last_exit_status = 0;
		}
		handle_line(shell, input);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_signals();
	shell.envp = dup_envp(envp);
	if (shell.envp == NULL)
	{
		perror("dup_envp");
		return (1);
	}
	shell.last_exit = 0;
	shell_loop(&shell);
	free_envp(shell.envp);
	return (shell.last_exit);
}
