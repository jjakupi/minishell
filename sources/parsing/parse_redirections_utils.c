/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:47:14 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 15:14:31 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	extend_in_files(t_command *cmd, char *clean)
{
	char	**old;
	char	**new;
	int		cnt;
	int		i;

	old = cmd->in_files;
	cnt = cmd->in_count;
	new = malloc((cnt + 1) * sizeof(char *));
	if (new == NULL)
		return (-1);
	i = 0;
	while (i < cnt)
	{
		new[i] = old[i];
		i++;
	}
	free(old);
	cmd->in_files = new;
	cmd->in_files[cnt] = clean;
	return (0);
}

int	push_input(t_command *cmd, const char *raw)
{
	char	*clean;
	int		ret;

	clean = remove_surrounding_quotes(raw);
	if (clean == NULL)
		return (-1);
	ret = extend_in_files(cmd, clean);
	if (ret < 0)
	{
		perror("malloc");
		free(clean);
		return (-1);
	}
	cmd->in_count++;
	return (0);
}

static int	extend_out_files(t_command *cmd, char *clean)
{
	char	**old;
	char	**new;
	int		cnt;
	int		i;

	old = cmd->out_files;
	cnt = cmd->out_count;
	new = malloc((cnt + 1) * sizeof(char *));
	if (new == NULL)
		return (-1);
	i = 0;
	while (i < cnt)
	{
		new[i] = old[i];
		i++;
	}
	free(old);
	cmd->out_files = new;
	cmd->out_files[cnt] = clean;
	return (0);
}

static int	extend_append_flags(t_command *cmd, int flag)
{
	int	*old;
	int	*new;
	int	cnt;
	int	i;

	old = cmd->append_flags;
	cnt = cmd->out_count;
	new = malloc((cnt + 1) * sizeof(int));
	if (new == NULL)
		return (-1);
	i = 0;
	while (i < cnt)
	{
		new[i] = old[i];
		i++;
	}
	free(old);
	cmd->append_flags = new;
	cmd->append_flags[cnt] = flag;
	return (0);
}

int	push_output(t_command *cmd, const char *raw, int append)
{
	char	*clean;
	int		ret;

	clean = remove_surrounding_quotes(raw);
	if (clean == NULL)
		return (-1);
	ret = extend_out_files(cmd, clean);
	if (ret < 0)
	{
		perror("malloc");
		free(clean);
		return (-1);
	}
	ret = extend_append_flags(cmd, append);
	if (ret < 0)
	{
		perror("malloc");
		free(clean);
		return (-1);
	}
	cmd->out_count++;
	return (0);
}
