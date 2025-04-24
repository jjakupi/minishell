/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:36:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/24 13:40:59 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_exit(t_command *cmd)
{
	long	code;

	printf("exit\n");
	if (cmd->arg_count == 0)
		exit(EXIT_SUCCESS);
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
		return (EXIT_FAILURE);
	}
	code = ft_atoi(cmd->args[0]);
	exit((unsigned char)code);
}
