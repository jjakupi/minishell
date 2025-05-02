/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:31:17 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 15:13:55 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;

void	run_builtin_or_exit(t_command *cmd)
{
	if (is_builtin(cmd->cmd))
		_exit(execute_builtin(cmd));
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

void	exec_external(t_command *cmd)
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
	execve(path, argv, environ);
	minishell_perror(cmd->cmd);
	if (errno == EACCES || errno == EISDIR)
		_exit(126);
	_exit(127);
}

void	child_exec_one(t_command *cmd, int in_fd, int out_fd)
{
	handle_empty(cmd);
	wire_pipes(in_fd, out_fd);
	apply_output_redirects(cmd);
	apply_input_redirects(cmd);
	apply_heredoc(cmd);
	run_builtin_or_exit(cmd);
	exec_external(cmd);
}
