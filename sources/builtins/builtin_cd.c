/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:17:31 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/21 16:06:05 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_cd(t_command *cmd, char ***env)
{
	char	cwd[MAX_PATH];

	if (cmd->arg_count > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd error");
		return (EXIT_FAILURE);
	}
	if (cmd->arg_count == 0 || strcmp(cmd->args[0], "~") == 0)
		return (change_dir_home(cwd, env));
	else if (strcmp(cmd->args[0], "-") == 0)
		return (change_to_old_dir(cwd, env));
	else if (cmd->args[0][0] == '\0')
	{
		ft_putstr_fd("cd: empty path\n", 2);
		return (EXIT_FAILURE);
	}
	return (execute_cd_path(cwd, cmd->args[0], env));
}
