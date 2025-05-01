/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_echo_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:34:46 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 15:26:47 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_echo_pipe(t_command *cmd, t_token **tok)
{
	if ((*tok)->type == PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_command(cmd);
		return (-1);
	}
	return (0);
}

t_command	*init_echo_command(void)
{
	t_command	*cmd;

	cmd = create_command();
	if (cmd == NULL)
		return (NULL);
	cmd->cmd = ft_strdup("echo");
	if (cmd->cmd == NULL)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
