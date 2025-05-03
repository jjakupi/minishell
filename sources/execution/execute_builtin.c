/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:43:23 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 12:33:52 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->cmd)
		return (-1);
	if (!ft_strcmp(cmd->cmd, "echo"))
		return (builtin_echo(cmd));
	else if (!ft_strcmp(cmd->cmd, "exit"))
		return (builtin_exit(cmd));
	else if (!ft_strcmp(cmd->cmd, "cd"))
		return (builtin_cd(cmd, &shell->envp));
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		return (builtin_pwd(cmd));
	else if (!ft_strcmp(cmd->cmd, "export"))
		return (builtin_export(cmd, &shell->envp));
	else if (!ft_strcmp(cmd->cmd, "env"))
		return (builtin_env(cmd, shell->envp));
	else if (!ft_strcmp(cmd->cmd, "unset"))
		return (builtin_unset(cmd, &shell->envp));
	return (127);
}
