#include "../include/minishell.h"

void process_special(const char *input, int *i, t_token **tokens, char **current_arg)
{
    // 1) finish any in-flight word
    flush_current_arg(tokens, current_arg);

    // 2) two-char operators
    if (input[*i] == '<' && input[*i+1] == '<') {
        add_token(tokens, new_token(HEREDOC, "<<"));
        *i += 2;
    }
    else if (input[*i] == '>' && input[*i+1] == '>') {
        add_token(tokens, new_token(REDIR_APPEND, ">>"));
        *i += 2;
    }
    // 3) single-char operators
    else if (input[*i] == '<') {
        add_token(tokens, new_token(REDIR_IN, "<"));
        (*i)++;
    }
    else if (input[*i] == '>') {
        add_token(tokens, new_token(REDIR_OUT, ">"));
        (*i)++;
    }
    else if (input[*i] == '|') {
        add_token(tokens, new_token(PIPE, "|"));
        (*i)++;
    }
    // 4) (if you ever add more operators, handle them here)
}


char *extract_special(const char *input, int *index)
{
	int		start;
	char	*special;

	start = *index;
	(*index)++;
	if ((input[start] == '>' || input[start] == '<') && input[*index] == input[start])
		(*index)++;
	special = ft_strndup(input + start, *index - start);
	if (!special)
		exit_with_error("ft_strndup (extract_special)");
	return (special);
}

t_token_type	get_special_token_type(const char *op)
{
	if (!ft_strcmp(op, "|"))
		return (PIPE);
	if (!ft_strcmp(op, "<"))
		return (REDIR_IN);
	if (!ft_strcmp(op, ">"))
		return (REDIR_OUT);
	if (!ft_strcmp(op, ">>"))
		return (REDIR_APPEND);
	if (!ft_strcmp(op, "<<"))
		return (HEREDOC);
	return (WORD);
}
