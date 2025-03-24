#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
#include "../../libft/libft.h"
// Define token types
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	ENV_VAR,
	END
}	t_token_type;

// Define the token structure (linked list)
typedef struct s_token
{
    int type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_command {
    char    *cmd;             // Command name (e.g., "echo")
    int     suppress_newline; // 1 if -n is provided, else 0
    char    **args;           // Array of argument strings
    int     arg_count;        // Number of arguments
	// New fields for redirections:
	char    *input_file;      // For '<'
	char    *output_file;     // For '>' or '>>'
	int     append_mode;      // 0 for '>', 1 for '>>'
	char    *heredoc_delimiter; // For '<<'
	int		has_heredoc;
	int		expand_heredoc;
	struct s_command *next;   // Next command in a pipeline
} t_command;


// Parse an echo command from a linked list of tokens
t_command *parse_echo(t_token *tokens);
t_command *parse_cd(t_token *tokens);
t_command *parse_pwd(t_token *tokens);
t_command *parse_export(t_token *tokens);
t_command *create_command(void);
t_command *parse_unset(t_token *tokens);
t_command *parse_env(t_token *tokens);
t_command *parse_exit(t_token *tokens);
t_command *parse_pipeline(t_token *tokens);
t_command *parse_single_command(t_token *tokens);
int parse_input_redirection(t_command *cmd, t_token **current);
int parse_output_redirection(t_command *cmd, t_token **current);
int	parse_heredoc(t_command *cmd, t_token **current);
int parse_append_redirection(t_command *cmd, t_token **current);
int	has_unmatched_quotes(const char *str);
int builtin_exit(t_command *cmd);
int is_valid_export_token(const char *str);
void free_command(t_command *cmd);
void add_argument(t_command *cmd, const char *arg);
int is_valid_identifier(const char *str);
int is_numeric(const char *str);
//EXPAND_VAR
char *get_env_value(const char *var);
char *expand_argument(const char *arg, int last_exit_status);
void expand_command_arguments(t_command *cmd, int last_exit_status);
//EXECUTION
int execute_builtin(t_command *cmd);
// Function Prototypes
t_token			*tokenize(const char *input);
t_token			*new_token(t_token_type type, const char *value);
void			add_token(t_token **head, t_token *new_tok);
void			free_tokens(t_token *head);
const char		*token_type_to_str(t_token_type type);
t_token_type	get_special_token_type(const char *op);
char			*extract_env_var(const char *input, int *index);
char			*extract_word(const char *input, int *index);
char			*extract_special(const char *input, int *index);
int				is_whitespace(char c);
int				is_special(char c);
int				is_invalid_character(char c);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strjoin3(const char *s1, const char *sep, const char *s2);
void			process_env_in_quotes(const char *input, int *index, int *start, t_token **head);
char			*append_str(char *str, const char *to_append);
char 			*append_char(char *str, char c);
void			flush_current_arg(t_token **tokens, char **current_arg);
void 			process_whitespace(int *i, t_token **tokens, char **current_arg);
void			process_special(const char *input, int *i, t_token **tokens, char **current_arg);
void			process_dollar(const char *input, int *i, t_token **tokens, char **current_arg);
void			process_quotes(const char *input, int *i, char **current_arg, t_token **tokens);

#endif
