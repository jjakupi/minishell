/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:35:52 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/21 15:35:54 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_env(t_command *cmd, char **env)
{
	int	i;

	i = 0;
	if (cmd->arg_count > 0)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
