/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:49 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/04 16:58:29 by jjakupi          ###   ########.fr       */
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

	if (ctx->in_single)
		return 0;
	if (ctx->src[ctx->idx] != '$')
		return 0;

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
		expand_dollar_env(ctx->src, &ctx->idx, ctx->buf, &ctx->pos, ctx->envp);
		return (1);
	}
	ctx->buf[ctx->pos++] = ctx->src[ctx->idx++];
	return (1);
}

void	handle_char_expansion(t_exp_ctx *ctx)
{
	char	c = ctx->src[ctx->idx];

	if (c == '\'' && ctx->in_double)
		ctx->buf[ctx->pos++] = c;
	else if (c == '\"' && ctx->in_single)
		ctx->buf[ctx->pos++] = c;
	else if (c != '\'' && c != '\"')
		ctx->buf[ctx->pos++] = c;
	ctx->idx++;
}


void	process_expansion(const char *s, int last_status, char *buf, char **envp)
{
	t_exp_ctx	ctx;

	ctx.src			= s;
	ctx.buf			= buf;
	ctx.idx			= 0;
	ctx.pos			= 0;
	ctx.last_status	= last_status;
	ctx.envp		= envp;
	ctx.in_single	= 0;
	ctx.in_double	= 0;

	while (ctx.src[ctx.idx] != '\0')
	{
		char c = ctx.src[ctx.idx];

		if (c == '\'' && !ctx.in_double)
		{
			ctx.in_single = !ctx.in_single;
			ctx.idx++;
			continue;
		}
		if (c == '"' && !ctx.in_single)
		{
			ctx.in_double = !ctx.in_double;
			ctx.idx++;
			continue;
		}
		if (!ctx.in_single && handle_dollar_expansion(&ctx))
			continue;
		handle_char_expansion(&ctx);
	}
	ctx.buf[ctx.pos] = '\0';
}

