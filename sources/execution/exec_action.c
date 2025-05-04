/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:31:17 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 18:10:35 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	run_builtin_or_exit(t_command *cmd, t_shell *shell)
{
	if (is_builtin(cmd->cmd))
		_exit(execute_builtin(cmd, shell));
}

char	**build_argv(t_command *cmd)
{
	char	**argv;
	int		total;
	int		i;

	total = cmd->arg_count + 2;
	argv = malloc(total * sizeof(*argv));
	if (argv == NULL)
		return (NULL);
	ft_bzero(argv, total * sizeof(*argv));
	argv[0] = cmd->cmd;
	i = 0;
	while (i < cmd->arg_count)
	{
		argv[i + 1] = cmd->args[i];
		i++;
	}
	argv[i + 1] = NULL;
	return (argv);
}

char	*resolve_path(const char *name)
{
	char	*path;

	if (ft_strchr(name, '/') != NULL)
	{
		if (access(name, F_OK) != 0)
		{
			minishell_perror(name);
			_exit(127);
		}
		return ((char *)name);
	}
	path = find_executable(name);
	if (!path)
	{
		write(2, "minishell: ", 11);
		write(2, name, strlen(name));
		write(2, ": command not found\n", 20);
		_exit(127);
	}
	return (path);
}

void exec_external(t_command *cmd, t_shell *shell)
{
	char		**argv;
	char		*path;
	struct stat	st;

	argv = build_argv(cmd);
	path = resolve_path(cmd->cmd);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putendl_fd(": Is a directory", 2);
		_exit(126);
	}
	execve(path, argv, shell->envp);
	minishell_perror(cmd->cmd);
	if (errno == EACCES || errno == EISDIR)
		_exit(126);
	_exit(127);
}

void	child_exec_one(t_command *cmd, int in_fd, int out_fd, t_shell *shell)
{
	handle_empty(cmd);
	wire_pipes(in_fd, out_fd);
	if (cmd->has_heredoc)
		apply_heredoc(cmd);
	apply_output_redirects(cmd);
	apply_input_redirects(cmd);
	run_builtin_or_exit(cmd, shell);
	exec_external(cmd, shell);
}
