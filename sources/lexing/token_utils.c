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
	else if (!ft_strcmp(op, "<"))
		return (REDIR_IN);
	else if (!ft_strcmp(op, ">"))
		return (REDIR_OUT);
	else if (!ft_strcmp(op, ">>"))
		return (REDIR_APPEND);
	else if (!ft_strcmp(op, "<<"))
		return (HEREDOC);
	return (WORD);
}

char	*extract_word(const char *input, int *index)
{
	int	start;

	start = *index;
	while (input[*index] && !is_whitespace(input[*index])
		&& !is_special(input[*index])
		&& input[*index] != '\'' && input[*index] != '\"')
		(*index)++;
	return (strndup(input + start, *index - start));
}

char	*extract_special(const char *input, int *index)
{
	int	start;

	start = *index;
	(*index)++;
	if ((input[start] == '>' || input[start] == '<') && input[*index] == input[start])
		(*index)++;
	return (strndup(input + start, *index - start));
}

char	*extract_env_var(const char *input, int *index)
{
	int	start;

	start = *index;
	(*index)++;
	if (input[*index] == '?' || input[*index] == '$')
	{
		(*index)++;
		return (strndup(input + start, *index - start));
	}
	if (!ft_isalpha(input[*index]) && input[*index] != '_')
		return (strdup("$"));
	while (input[*index] && (ft_isalnum(input[*index]) || input[*index] == '_'))
		(*index)++;
	return (strndup(input + start, *index - start));
}

void	process_env_in_quotes(const char *input, int *index, int *start, t_token **head)
{
	char	*literal;
	char	*env_var;

	literal = strndup(input + *start, *index - *start);
	if (literal && *literal)
		add_token(head, new_token(WORD, literal));
	free(literal);
	env_var = extract_env_var(input, index);
	if (env_var)
	{
		add_token(head, new_token(ENV_VAR, env_var));
		free(env_var);
	}
	*start = *index;
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
	char	*result;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(sep) + ft_strlen(s2) + 1;
	result = malloc(len);
	if (!result)
		return (NULL);
	strcpy(result, s1);
	strcat(result, sep);
	strcat(result, s2);
	return (result);
}
