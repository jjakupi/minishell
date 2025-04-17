#include "../include/minishell.h"

int process_quotes(const char *input, int *i, t_token **tokens)
{
    char quote = input[(*i)++];    // ' or "
    char *inner = NULL;

    // collect up to the matching quote
    while (input[*i] && input[*i] != quote)
        inner = append_char(inner, input[(*i)++]);

    // if we never saw the closer → syntax error
    if (input[*i] != quote)
    {
        free(inner);
        return syntax_error("unmatched quote");
    }
    (*i)++;  // skip closing quote

    // build a new string that still has its quotes around it
    int len = inner ? strlen(inner) : 0;
    char *val = malloc(len + 3);
    if (!val) exit_with_error("malloc");
    val[0]            = quote;
    if (inner) memcpy(val+1, inner, len);
    val[len+1]        = quote;
    val[len+2]        = '\0';
    free(inner);

    add_token(tokens, new_token(WORD, val));
    free(val);
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

