/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exand_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 13:27:23 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_dollar_question(int status, char *buf, int *pos)
{
	char	*num;
	int		n;

	num = ft_itoa(status);
	if (num == NULL)
		return ;
	n = ft_strlen(num);
	ft_strlcpy(buf + *pos, num, n + 1);
	*pos += n;
	free(num);
}

void	expand_dollar_pid(char *buf, int *pos)
{
	char	*num;
	int		n;

	num = ft_itoa((int)getpid());
	if (num == NULL)
		return ;
	n = ft_strlen(num);
	ft_strlcpy(buf + *pos, num, n + 1);
	*pos += n;
	free(num);
}

void	expand_dollar_env(const char *src, int *idx, char *buf, int *pos, char **envp)
{
	char	var[256];
	int		k;
	char	*val;

	k = 0;
	(*idx)++;
	while ((ft_isalnum(src[*idx]) || src[*idx] == '_') && k < 255)
		var[k++] = src[(*idx)++];
	var[k] = '\0';
	val = get_env_value(envp, var);
	if (val)
	{
		ft_strlcpy(buf + *pos, val, ft_strlen(val) + 1);
		*pos += ft_strlen(val);
	}
}
