/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:16:25 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/24 13:28:15 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[MAX_PATH];

	(void)cmd;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	return (EXIT_SUCCESS);
}
