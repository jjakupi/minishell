/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:36:09 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/21 16:13:45 by julrusse         ###   ########.fr       */
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
		fprintf(stderr, "bash: exit: %s: numeric argument required\n", //fprintf not ok
			cmd->args[0]);
		exit(255);
	}
	if (cmd->arg_count > 1)
	{
		fprintf(stderr, "bash: exit: too many arguments\n"); //fprintf not ok
		return (EXIT_FAILURE);
	}
	code = ft_atoi(cmd->args[0]);
	exit((unsigned char)code);
}
