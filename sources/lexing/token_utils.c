#include "../include/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
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

char	*extract_word(const char *input, int *index)
{
	int	start = *index;

	while (input[*index] && !is_whitespace(input[*index])
		&& !is_special(input[*index])
		&& input[*index] != '\'' && input[*index] != '\"')
		(*index)++;

	char *word = strndup(input + start, *index - start);
	if (!word)
	{
		perror("strndup failed");
		exit(EXIT_FAILURE);
	}
	return word;
}

char	*extract_special(const char *input, int *index)
{
	int	start = *index;
	(*index)++;

	if ((input[start] == '>' || input[start] == '<') && input[*index] == input[start])
		(*index)++;

	char *special = strndup(input + start, *index - start);
	if (!special)
	{
		perror("strndup failed");
		exit(EXIT_FAILURE);
	}
	return special;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*ft_strjoin3(const char *s1, const char *sep, const char *s2)
{
	size_t	len = ft_strlen(s1) + ft_strlen(sep) + ft_strlen(s2) + 1;
	char	*result = malloc(len);
	if (!result)
	{
		perror("malloc failed in ft_strjoin3");
		exit(EXIT_FAILURE);
	}
	strcpy(result, s1);
	strcat(result, sep);
	strcat(result, s2);
	return result;
}
