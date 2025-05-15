/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/15 16:59:05 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_dollar_expansion(t_exp_ctx *ctx)
{
	char	c;

	if (ctx->in_single)
		return (0);
	if (ctx->src[ctx->idx] != '$')
		return (0);
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
		return (expand_dollar_env(ctx), 1);
	ctx->buf[ctx->pos++] = ctx->src[ctx->idx++];
	return (1);
}

void	handle_char_expansion(t_exp_ctx *ctx)
{
	char	c;

	c = ctx->src[ctx->idx];
	if (c == '\'' && ctx->in_double)
		ctx->buf[ctx->pos++] = c;
	else if (c == '\"' && ctx->in_single)
		ctx->buf[ctx->pos++] = c;
	else if (c != '\'' && c != '\"')
		ctx->buf[ctx->pos++] = c;
	ctx->idx++;
}

static int	process_quotes_exp(t_exp_ctx *ctx)
{
	char	c;

	c = ctx->src[ctx->idx];
	if (c == '\'' && !ctx->in_double)
	{
		ctx->in_single = !ctx->in_single;
		ctx->idx++;
		return (1);
	}
	if (c == '"' && !ctx->in_single)
	{
		ctx->in_double = !ctx->in_double;
		ctx->idx++;
		return (1);
	}
	return (0);
}

void	process_expansion(const char *s, int last_status, char *buf,
			char **envp)
{
	t_exp_ctx	ctx;

	ctx.src = s;
	ctx.buf = buf;
	ctx.idx = 0;
	ctx.pos = 0;
	ctx.last_status = last_status;
	ctx.envp = envp;
	ctx.in_single = 0;
	ctx.in_double = 0;
	while (ctx.src[ctx.idx] != '\0')
	{
		if (process_quotes_exp(&ctx))
			continue ;
		if (!ctx.in_single && handle_dollar_expansion(&ctx))
			continue ;
		handle_char_expansion(&ctx);
	}
	ctx.buf[ctx.pos] = '\0';
}
