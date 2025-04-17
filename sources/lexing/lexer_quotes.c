#include "../include/minishell.h"

int process_quotes(const char *input, int *i, char **current_arg)
{
    char  quote = input[(*i)++];      // ' or "
    int   start = *i;
    char *inner;
    char *wrapped;
    int   ilen, clen;

    // 1) Find the closing quote
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
        return syntax_error("unmatched quote");

    // 2) Grab the inner text
    ilen = *i - start;
    inner = malloc(ilen + 1);
    if (!inner) exit_with_error("malloc");
    memcpy(inner, input + start, ilen);
    inner[ilen] = '\0';

    // 3) Build a small buffer "'inner'" or "\"inner\""
    wrapped = malloc(ilen + 3);
    if (!wrapped) exit_with_error("malloc");
    wrapped[0]        = quote;
    memcpy(wrapped + 1, inner, ilen);
    wrapped[ilen + 1] = quote;
    wrapped[ilen + 2] = '\0';
    free(inner);

    // 4) Append it onto current_arg
    if (!*current_arg)
    {
        *current_arg = strdup(wrapped);
        if (!*current_arg) exit_with_error("strdup");
    }
    else
    {
        clen = strlen(*current_arg);
        char *newarg = malloc(clen + strlen(wrapped) + 1);
        if (!newarg) exit_with_error("malloc");
        memcpy(newarg, *current_arg, clen);
        memcpy(newarg + clen, wrapped, strlen(wrapped) + 1);
        free(*current_arg);
        *current_arg = newarg;
    }
    free(wrapped);

    // 5) Skip the closing quote and return
    (*i)++;
    return 0;
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void process_whitespace(int *i, t_token **tokens, char **current_arg)
{
	flush_current_arg(tokens, current_arg);
	(*i)++;
}

void flush_current_arg(t_token **tokens, char **current_arg)
{
	if (*current_arg)
	{
		add_token(tokens, new_token(WORD, *current_arg));
		free(*current_arg);
		*current_arg = NULL;
	}
}

