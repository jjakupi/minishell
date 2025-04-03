#include "../include/minishell.h"

void	exit_with_error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	syntax_error(const char *unexpected_token)
{
	printf("minishell: syntax error near unexpected token `%s'\n", unexpected_token);
	return (1);
}


char *ft_strndup(const char *s, size_t n)
{
	size_t	len = 0;
	char	*dup;

	while (len < n && s[len])
		len++;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		dup[i] = s[i];
	dup[len] = '\0';
	return (dup);
}

char *ft_strcpy(char *dest, const char *src)
{
	char *start = dest;

	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (start);
}

char *append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	if (str != NULL)
		len = strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (!new_str)
		exit_with_error("malloc (append_char)");
	if (str != NULL)
	{
		ft_strcpy(new_str, str);
		free(str);
	}
	else
		new_str[0] = '\0';
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}


