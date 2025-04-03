#include "../include/minishell.h"

void process_special(const char *input, int *i, t_token **tokens, char **current_arg)
{
	flush_current_arg(tokens, current_arg);
	char *spec = extract_special(input, i);
	add_token(tokens, new_token(get_special_token_type(spec), spec));
	free(spec);
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
