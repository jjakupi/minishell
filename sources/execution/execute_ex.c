/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:47:36 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 14:45:17 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	handle_heredoc_redir(t_command *cmd, int *old_stdin)
{
	int	hp[2];

	*old_stdin = dup(STDIN_FILENO);
	if (*old_stdin < 0)
		return (minishell_perror("dup"), 1);
	if (pipe(hp) < 0)
	{
		minishell_perror("pipe");
		safe_close(*old_stdin);
		return (1);
	}
	read_heredoc_lines(cmd->heredoc_delimiter, hp[1]);
	safe_close(hp[1]);
	dup2(hp[0], STDIN_FILENO);
	safe_close(hp[0]);
	return (0);
}
