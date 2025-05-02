/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:36:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 15:02:42 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_exit(t_command *cmd)
{
	if (cmd->arg_count == 0)
		exit(0);
	if (!is_numeric(cmd->args[0]))
	{
		write(2, "bash: exit: ", 12);
		write(2, cmd->args[0], strlen(cmd->args[0]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
	if (cmd->arg_count > 1)
	{
		write(2, "bash: exit: too many arguments\n", 31);
		return 1;
	}
	exit((unsigned char)atoi(cmd->args[0]));
}
