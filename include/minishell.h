/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:56:55 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/01 11:02:49 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Terminal prompt color definitions
# define GREEN "\001\033[1;32m\002"
# define RESET "\001\033[0m\002"
# define PROMPT GREEN "Minishell:~$ " RESET
# define MAX_PATH 4096

// Standard includes
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/stat.h>
# include "../libft/libft.h"

// Global signal handler variable
extern volatile sig_atomic_t	g_signal;

// Token types
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

// Token structure
typedef struct s_token
{
	int					type;
	char				*value;
	struct s_token		*next;
}	t_token;

// Command structure
typedef struct s_command
{
    char   *cmd;
    int     suppress_newline;

    // argv
    char  **args;
    int     arg_count;

    // all '<' redirections, in parse order
    char  **in_files;
    int     in_count;

    // all '>' / '>>' redirections, in parse order
    char  **out_files;
    int     out_count;
    int    *append_flags;  // parallel array: 0=truncate, 1=append

    // here-doc
    char   *heredoc_delimiter;
    int     has_heredoc;
    int     expand_heredoc;

    struct s_command *next;
} t_command;

// Lexing (Tokenization) Functions
t_token			*tokenize(const char *input);
t_token			*new_token(t_token_type type, const char *value);
void			add_token(t_token **head, t_token *new_tok);
void			free_tokens(t_token *head);
int				is_whitespace(char c);
int				is_special(char c);
char			*append_char(char *str, char c);
void			flush_current_arg(t_token **tokens, char **current_arg);
void			process_whitespace(int *i, t_token **tokens,
					char **current_arg);
int				handle_double_special(const char *input, int *i, t_token **tokens);
void			handle_single_special(const char *input, int *i, t_token **tokens);
void			process_special(const char *input, int *i, t_token **tokens,
					char **current_arg);
char			*build_wrapped(const char *start_ptr, int len, char quote);
void			append_wrapped(char **current_arg, const char *wrapped);
int				process_quotes(const char *input, int *i, char **current_arg);
char			*extract_word(const char *input, int *index);
char			*extract_special(const char *input, int *index);
t_token_type	get_special_token_type(const char *op);

// Parsing Functions
t_command		*parse_single_command(t_token *tokens);
int				parse_pipeline(t_token *tokens, t_command **result);
t_command		*create_command(void);
void			add_argument(t_command *cmd, const char *arg);
void			free_command(t_command *cmd);
int				check_syntax_errors(t_token *tokens);
int				check_next_token(t_token *current, char **value);
int				handle_token_parsing(t_command *cmd, t_token **tokens);
int				has_unmatched_quotes(const char *str);
char			*remove_surrounding_quotes(const char *str);
void            minishell_perror(const char *what);

// Redirection Handling
int				is_redirection(t_token_type type);
int				parse_redirections(t_command *cmd, t_token **tokens);
int				parse_input_redirection(t_command *cmd, t_token **current);
int				parse_output_redirection(t_command *cmd, t_token **current);
int				parse_append_redirection(t_command *cmd, t_token **current);
int				parse_heredoc(t_command *cmd, t_token **current);
int				set_redirection_file(char **dest, char *src);

// Built-in Commands
t_command		*parse_echo(t_token *tokens);
t_command		*parse_cd(t_token *tokens);
t_command		*parse_pwd(t_token *tokens);
t_command		*parse_export(t_token *tokens);
t_command		*parse_unset(t_token *tokens);
t_command		*parse_env(t_token *tokens);
t_command		*parse_exit(t_token *tokens);

int				execute_builtin(t_command *cmd);
int				builtin_exit(t_command *cmd);
int				builtin_echo(t_command *cmd);
int				builtin_env(t_command *cmd, char **env);
int				builtin_pwd(t_command *cmd);
int				builtin_cd(t_command *cmd, char ***env);
int				builtin_export(t_command *cmd, char ***env);
int				builtin_unset(t_command *cmd, char ***env);
int				is_builtin(char *cmd);

//unset Utils
int				env_len(char **env);
int				env_idx(char **env, const char *key);
int				env_remove_var(char ***env_ptr, const char *key);
int				valid_unset_identifier(const char *token);

//cd Utils
int				execute_cd_path(char *cur_pwd, char *path, char ***env);
int				change_to_old_dir(char *cur_pwd, char ***env);
int				change_dir_home(char *cur_pwd, char ***env);
int				update_directories(const char *old_pwd, char ***env);

//export Utils
int				env_op(char **env, const char *key, int mode);
int				is_valid_identifier_export(const char *token);
int				cmpfunc(const void *a, const void *b);
char			**copy_env(char **env);
void			sort_env(char **arr);
void			print_entry(const char *var);
void			print_sorted_env(char **env);
void			extract_key(const char *assignment, char *key);
int				add_entry(char ***env_ptr, const char *entry);
int				update_entry(char **env, int idx, const char *assignment);
int				set_env_var(char ***env_ptr, const char *assignment);

// Variable Expansion
char			*get_env_value(const char *var);
char			*expand_argument(const char *arg, int last_exit_status);
void			expand_command_arguments(t_command *cmd, int last_exit_status);
void			normalize_empty_cmd(t_command *c);

// Command Execution
int execute_command(t_command *cmd);
int exec_pipeline(t_command *head);

// Utility Functions
void			exit_with_error(const char *msg);
int				syntax_error(const char *unexpected_token);
int				is_valid_export_token(const char *str);
int				is_valid_identifier(const char *str);
int				is_numeric(const char *str);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strndup(const char *s, size_t n);
int				ft_strcmp(const char *s1, const char *s2);

#endif
