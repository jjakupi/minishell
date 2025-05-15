/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exand_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 16:48:13 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_entirely_single_quoted(const char *arg, int len)
{
	if (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'')
		return (1);
	return (0);
}

char	*handle_single_quotes(const char *arg, int len)
{
	return (ft_substr(arg, 1, len - 2));
}

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

void	expand_dollar_env(t_exp_ctx *ctx)
{
	char	var[256];
	int		k;
	char	*val;

	k = 0;
	ctx->idx++;
	while ((ft_isalnum(ctx->src[ctx->idx]) || ctx->src[ctx->idx] == '_')
		&& k < 255)
	{
		var[k++] = ctx->src[ctx->idx++];
	}
	var[k] = '\0';
	val = get_env_value(ctx->envp, var);
	if (val)
	{
		ft_strlcpy(ctx->buf + ctx->pos, val, ft_strlen(val) + 1);
		ctx->pos += ft_strlen(val);
	}
}
