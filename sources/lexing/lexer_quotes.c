#include "../include/minishell.h"

int process_quotes(const char *input, int *i, char **current_arg)
{
	char	quote;
	char	*temp;
	char	*new_arg;

	quote = input[(*i)++];
	temp = NULL;
	while (input[*i] && input[*i] != quote)
		temp = append_char(temp, input[(*i)++]);

	if (input[*i] != quote)
	{
		free(temp);
		return (syntax_error("unmatched quote"));
	}
	(*i)++;

	if (!temp)
		temp = ft_strdup("");

	if (*current_arg)
	{
		new_arg = ft_strjoin(*current_arg, temp);
		if (!new_arg)
			exit_with_error("ft_strjoin");
		free(*current_arg);
		*current_arg = new_arg;
	}
	else
	{
		*current_arg = ft_strdup(temp);
		if (!*current_arg)
			exit_with_error("ft_strdup");
	}

	free(temp);
	return (0);
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

