/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exand_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 14:03:41 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_dollar_question(int status, char *buf, int *pos)
{
	char	*num;
	int		n;

	num = ft_itoa(status);
	if (num == NULL)
		return;
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
		return;
	n = ft_strlen(num);
	ft_strlcpy(buf + *pos, num, n + 1);
	*pos += n;
	free(num);
}

void	expand_dollar_env(const char *s, int *idx, char *buf, int *pos)
{
	char	var[256];
	int		k;

	k = 0;
	(*idx)++;
	while ((ft_isalnum(s[*idx]) || s[*idx] == '_') && k < 255)
		var[k++] = s[(*idx)++];
	var[k] = '\0';
	s = getenv(var);
	if (s)
	{
		ft_strlcpy(buf + *pos, s, ft_strlen(s) + 1);
		*pos += ft_strlen(s);
	}
}
