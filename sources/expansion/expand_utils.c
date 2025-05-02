/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/02 13:59:35 by jjakupi          ###   ########.fr       */
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

int	handle_dollar_expansion(t_exp_ctx *ctx)
{
	char	c;

	if (ctx->src[ctx->idx] != '$')
		return (0);
	c = ctx->src[ctx->idx + 1];
	if (c == '?')
	{
		expand_dollar_question(ctx->last_status, ctx->buf, &ctx->pos);
		ctx->idx += 2;
		return (1);
	}
	if (c == '$')
	{
		expand_dollar_pid(ctx->buf, &ctx->pos);
		ctx->idx += 2;
		return (1);
	}
	if (ft_isalpha(c) || c == '_')
	{
		expand_dollar_env(ctx->src, &ctx->idx, ctx->buf, &ctx->pos);
		return (1);
	}
	ctx->buf[ctx->pos++] = ctx->src[ctx->idx++];
	return (1);
}

void	handle_char_expansion(t_exp_ctx *ctx)
{
	char	c;

	c = ctx->src[ctx->idx];
	if (c != '\'' && c != '"')
		ctx->buf[ctx->pos++] = c;
	ctx->idx++;
}

void	process_expansion(const char *s, int last_status, char *buf)
{
	t_exp_ctx	ctx;

	ctx.src = s;
	ctx.buf = buf;
	ctx.idx = 0;
	ctx.pos = 0;
	ctx.last_status = last_status;
	while (ctx.src[ctx.idx] != '\0')
	{
		if (handle_dollar_expansion(&ctx))
			continue ;
		handle_char_expansion(&ctx);
	}
	ctx.buf[ctx.pos] = '\0';
}
