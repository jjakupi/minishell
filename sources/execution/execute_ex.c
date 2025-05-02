/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:47:36 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 13:59:35 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;

void	minishell_perror(const char *what)
{
	const char	*msg;

	msg = strerror(errno);
	write(2, "minishell: ", 11);
	write(2, what, strlen(what));
	write(2, ": ", 2);
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
}

char	*test_path_segment(char **p_ptr, const char *name)
{
	char	*segment;
	int		len;
	char	*full;

	segment = *p_ptr;
	len = 0;
	while (segment[len] != '\0' && segment[len] != ':')
		len++;
	if (len > 0)
	{
		full = malloc(len + 1 + ft_strlen(name) + 1);
		if (full == NULL)
			return (NULL);
		ft_strlcpy(full, segment, len + 1);
		full[len] = '/';
		ft_strlcpy(full + len + 1, name, ft_strlen(name) + 1);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
	}
	*p_ptr = segment + len;
	if (**p_ptr == ':')
		(*p_ptr)++;
	return (NULL);
}

char	*find_executable(const char *name)
{
	char	*path_env;
	char	*paths;
	char	*p;
	char	*result;

	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);
	paths = ft_strdup(path_env);
	if (paths == NULL)
		return (NULL);
	p = paths;
	result = NULL;
	while (result == NULL && *p != '\0')
		result = test_path_segment(&p, name);
	free(paths);
	return (result);
}

int	exec_single(t_command *cmd)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	pid = fork();
	if (pid < 0)
	{
		minishell_perror("fork");
		return (1);
	}
	if (pid == 0)
		child_exec_one(cmd, -1, -1);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = 1;
	return (exit_code);
}
