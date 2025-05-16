/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:36:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/16 11:58:59 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_exit(t_command *cmd)
{
	if (cmd->arg_count == 0)
		exit(0);
	if (!is_numeric(cmd->args[0]))
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (cmd->arg_count > 1)
	{
		ft_putendl_fd("bash: exit: too many arguments", 2);
		return (1);
	}
	exit((unsigned char)ft_atoi(cmd->args[0]));
}
