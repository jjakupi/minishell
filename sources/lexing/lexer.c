#include "../include/minishell.h"

t_token	*tokenize(const char *input)
{
	int			i = 0;
	t_token		*tokens = NULL;
	char		*current_arg = NULL;

	while (input[i])
	{
		if (is_whitespace(input[i]))
			process_whitespace(&i, &tokens, &current_arg);
		else if (is_special(input[i]))
			process_special(input, &i, &tokens, &current_arg);
		else if (input[i] == '\'' || input[i] == '"')
		{
			if (process_quotes(input, &i, &current_arg))
			{
				free(current_arg);
				free_tokens(tokens);
				return NULL;
			}
		}
		else
			current_arg = append_char(current_arg, input[i++]);
	}
	flush_current_arg(&tokens, &current_arg);
	return tokens;
}

int process_quotes(const char *input, int *i, char **current_arg)
{
	char quote = input[(*i)++];
	char *temp = NULL;

	while (input[*i] && input[*i] != quote)
		temp = append_char(temp, input[(*i)++]);

	if (input[*i] != quote)
	{
		fprintf(stderr, "minishell: Syntax error unmatched '%c'\n", quote);
		free(temp);
		return 1;
	}
	(*i)++;

	if (!temp) // Explicitly handle empty quotes
		temp = strdup("");

	// Append the quoted content directly to the current_arg clearly
	if (*current_arg)
	{
		char *new_arg = ft_strjoin(*current_arg, temp);
		free(*current_arg);
		*current_arg = new_arg;
	}
	else
	{
		*current_arg = strdup(temp);
	}

	free(temp);
	return 0;
}

// Process special characters (|, <, >, >>, <<)
void	process_special(const char *input, int *i, t_token **tokens, char **current_arg)
{
	flush_current_arg(tokens, current_arg);
	char *spec = extract_special(input, i);
	add_token(tokens, new_token(get_special_token_type(spec), spec));
	free(spec);
}

// Process whitespace
void	process_whitespace(int *i, t_token **tokens, char **current_arg)
{
	flush_current_arg(tokens, current_arg);
	(*i)++;
}

// Flush current argument into a token
void	flush_current_arg(t_token **tokens, char **current_arg)
{
	if (*current_arg)
	{
		add_token(tokens, new_token(WORD, *current_arg));
		free(*current_arg);
		*current_arg = NULL;
	}
}

// Append a character safely to a dynamically allocated string
char	*append_char(char *str, char c)
{
	size_t	len = str ? strlen(str) : 0;
	char	*new_str = malloc(len + 2);

	if (!new_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (str)
	{
		strcpy(new_str, str);
		free(str);
	}
	else
		new_str[0] = '\0';
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return new_str;
}

// Append a string safely to another dynamically allocated string
char	*append_str(char *str, const char *to_append)
{
	size_t len1 = str ? strlen(str) : 0;
	size_t len2 = strlen(to_append);
	char *new_str = malloc(len1 + len2 + 1);

	if (!new_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (str)
	{
		strcpy(new_str, str);
		free(str);
	}
	else
		new_str[0] = '\0';
	strcat(new_str, to_append);
	return new_str;
}

