#include "../includes/minishell.h"

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
			process_quotes(input, &i, &current_arg, &tokens);
			i++;
		}
		else if (input[i] == '$')
			process_dollar(input, &i, &tokens, &current_arg);
		else
			current_arg = append_char(current_arg, input[i++]);
	}
	if (current_arg && *current_arg)
		flush_current_arg(&tokens, &current_arg);
	else
		free(current_arg);
	return tokens;
}

void	process_dollar(const char *input, int *i, t_token **tokens, char **current_arg)
{
	char	*env;

	if (*current_arg)
		flush_current_arg(tokens, current_arg);
	env = extract_env_var(input, i);
	if (env && ft_strlen(env) == 1 && env[0] == '$') // Single standalone $
	{
		add_token(tokens, new_token(WORD, env));
		free(env);
	}
	else
	{
		add_token(tokens, new_token(ENV_VAR, env));
		free(env);
	}
}

void	process_quotes(const char *input, int *i, char **current_arg, t_token **tokens)
{
    char quote = input[(*i)++]; // Save and skip the opening quote.

    // Check if we are processing a double quote.
    int is_double_quote = (quote == '"');

    // Collect characters until we hit the matching quote.
    while (input[*i] && input[*i] != quote)
    {
        // If inside double quotes, check for a '$' that signals an environment variable.
        if (is_double_quote && input[*i] == '$')
        {
            // Flush any literal text collected so far.
            flush_current_arg(tokens, current_arg);
            // Process the dollar variable.
            process_dollar(input, i, tokens, current_arg);
            // Continue without incrementing *i here because process_dollar updates it.
            continue;
        }
        else
        {
            *current_arg = append_char(*current_arg, input[*i]);
            (*i)++;
        }
    }

    // If a closing quote is found, skip it.
    if (input[*i] == quote)
        (*i)++;

    // After finishing, check the next character.
    if (!input[*i] || is_whitespace(input[*i]) || is_special(input[*i]))
    {
        flush_current_arg(tokens, current_arg);
    }
}

void	process_special(const char *input, int *i, t_token **tokens, char **current_arg)
{
	char	*spec;

	flush_current_arg(tokens, current_arg);
	spec = extract_special(input, i);
	add_token(tokens, new_token(get_special_token_type(spec), spec));
	free(spec);
}

void process_whitespace(int *i, t_token **tokens, char **current_arg)
{
	if (*current_arg)
	{
		add_token(tokens, new_token(WORD, *current_arg));
		free(*current_arg);
		*current_arg = NULL;
	}
	(*i)++;
}


void	flush_current_arg(t_token **tokens, char **current_arg)
{
	if (*current_arg)
	{
		add_token(tokens, new_token(WORD, *current_arg));
		free(*current_arg);
		*current_arg = NULL;
	}
}

char *append_char(char *str, char c)
{
    size_t len = str ? strlen(str) : 0;
    char *new_str = malloc(len + 2); // +1 for new char, +1 for null terminator
    if (!new_str)
        return NULL;
    if (str)
    {
        strcpy(new_str, str);
        free(str);
    }
    else
    {
        new_str[0] = '\0';
    }
    new_str[len] = c;
    new_str[len + 1] = '\0';
    return new_str;
}

/* Append a string to an existing string.
 * If 'str' is NULL, a duplicate of 'to_append' is returned.
 * Returns a newly allocated string containing the concatenated result.
 */
char *append_str(char *str, const char *to_append)
{
    size_t len1 = str ? strlen(str) : 0;
    size_t len2 = strlen(to_append);
    char *new_str = malloc(len1 + len2 + 1); // +1 for null terminator
    if (!new_str)
        return NULL;
    if (str)
    {
        strcpy(new_str, str);
        free(str);
    }
    else
    {
        new_str[0] = '\0';
    }
    strcat(new_str, to_append);
    return new_str;
}
