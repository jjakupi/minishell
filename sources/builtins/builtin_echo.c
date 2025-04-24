/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:33:52 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/21 15:35:23 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	suppress_newline;

	if (!cmd || !cmd->cmd)
		return (-1);
	suppress_newline = 0;
	i = 0;
	if (cmd->arg_count > 0 && ft_strcmp(cmd->args[0], "-n") == 0)
	{
		suppress_newline = 1;
		i++;
	}
	while (i < cmd->arg_count)
	{
		printf("%s", cmd->args[i]);
		if (i < cmd->arg_count - 1)
			printf(" ");
		i++;
	}
	if (!suppress_newline)
		printf("\n");
	return (EXIT_SUCCESS);
}
