/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjakupi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:56:55 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/16 11:37:57 by jjakupi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Terminal prompt color definitions
# define PROMPT "\001\033[1;32m\002Minishell:~$ \001\033[0m\002"
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
extern volatile sig_atomic_t	g_last_exit_status;

// Environement pointer structure
typedef struct s_shell
{
	char	**envp;
	int		last_exit;
	int		(*pipes)[2];
	int		pipe_count;
}	t_shell;

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
	char				*cmd;
	int					suppress_newline;
	char				**args;
	int					arg_count;
	char				**in_files;
	int					in_count;
	char				**out_files;
	int					out_count;
	int					*append_flags;
	char				*heredoc_delimiter;
	int					has_heredoc;
	int					expand_heredoc;
	int					heredoc_fd;
	struct s_command	*next;
}	t_command;

// Expansion structure
typedef struct s_exp_ctx
{
	const char	*src;
	char		*buf;
	int			idx;
	int			pos;
	int			last_status;
	char		**envp;
	int			in_single;
	int			in_double;
}	t_exp_ctx;

static inline void	safe_close(int fd)
{
	if (fd >= 0)
		close(fd);
}

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
int				handle_double_special(const char *input,
						int *i, t_token **tokens);
void			handle_single_special(const char *input,
						int *i, t_token **tokens);
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
char			**alloc_new_args(char **old_args, int old_count);
void			add_argument(t_command *cmd, const char *arg);
void			free_string_array(char **arr, int count);
void			free_command(t_command *cmd);
int				check_syntax_errors(t_token *tokens);
int				check_next_token(t_token *current, char **value);
int				handle_token_parsing(t_command *cmd, t_token **tokens);
int				has_unmatched_quotes(const char *str);
char			*remove_surrounding_quotes(const char *str);
void			minishell_perror(const char *what);
int				push_input(t_command *cmd, const char *raw);
int				push_output(t_command *cmd, const char *raw, int append);
int				handle_echo_pipe(t_command *cmd, t_token **tok);
t_command		*init_echo_command(void);

// Redirection Handling
int				is_redirection(t_token_type type);
int				parse_redirections(t_command *cmd, t_token **tokens);
int				parse_input_redirection(t_command *cmd, t_token **current);
int				parse_output_redirection(t_command *cmd, t_token **current);
int				parse_append_redirection(t_command *cmd, t_token **current);
int				parse_heredoc(t_command *cmd, t_token **current);
int				set_redirection_file(char **dest, char *src);
void			read_heredoc_lines(const char *delim, int write_end);

// Built-in Commands
t_command		*parse_echo(t_token *tokens);
t_command		*parse_cd(t_token *tokens);
t_command		*parse_pwd(t_token *tokens);
t_command		*parse_export(t_token *tokens);
t_command		*parse_unset(t_token *tokens);
t_command		*parse_env(t_token *tokens);
t_command		*parse_exit(t_token *tokens);
int				execute_builtin(t_command *cmd, t_shell *shell);
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
char			*get_append_key(const char *arg);
char			*get_append_suffix(const char *arg);
char			*build_appended_entry(char ***env, int idx,
						const char *key, const char *suffix);

// Variable Expansion
char			*get_env_value(char **envp, const char *key);
char			*expand_argument(const char *arg, int last_exit_status,
						char **envp);
void			expand_command_arguments(t_command *cmd, int last_exit_status,
						char **envp);
void			normalize_empty_cmd(t_command *c);
void			fix_empty_cmd(t_command *c);
int				is_entirely_single_quoted(const char *arg, int len);
char			*handle_single_quotes(const char *arg, int len);
void			expand_dollar_question(int status, char *buf, int *pos);
void			expand_dollar_pid(char *buf, int *pos);
void			expand_dollar_env(t_exp_ctx *ctx);
int				handle_dollar_expansion(t_exp_ctx *ctx);
void			handle_char_expansion(t_exp_ctx *ctx);
void			process_expansion(const char *s, int last_status,
						char *buf, char **envp);
void			init_shlvl(void);

/* exec_helper.c */
void			handle_empty(t_command *cmd);
void			wire_pipes(int in_fd, int out_fd);
void			apply_output_redirects(t_command *cmd);
void			apply_input_redirects(t_command *cmd);
void			apply_heredoc(t_command *cmd);
int				open_and_dup_output(char *file, int append);

/* exec_action.c */
void			run_builtin_or_exit(t_command *cmd, t_shell *shell);
char			**build_argv(t_command *cmd);
char			*resolve_path(const char *name);
void			exec_external(t_command *cmd, t_shell *shell);
void			child_exec_one(t_command *cmd, int in_fd, int out_fd,
						t_shell *shell);
int				handle_heredoc_redir(t_command *cmd, int *old_stdin);
int				exec_single(t_command *cmd, t_shell *shell);
void			child_redirect_heredoc(int read_end);

// Pipeline
int				count_stages(t_command *head);
t_command		**build_stage_array(t_command *head, int n);
void			make_pipes(int (*pipes)[2], int n);
int				wait_for_children(pid_t *pids, int n);
void			spawn_pipeline(t_command **st, pid_t *pids, t_shell *shell);
void			pipeline_child(t_command *cmd, int in_fd, int out_fd,
						t_shell *shell);
void			close_unused_pipes(int (*pipes)[2], int count, int in_fd,
						int out_fd);
void			close_all_pipes(int (*pipes)[2], int count);
int				alloc_pipeline_resources(t_command *head, t_command ***stp,
						int (**pipes)[2], pid_t **pidsp);
void			free_pipeline_resources(t_command **st, int (*pipes)[2],
						pid_t *pids);

// Command Execution
int				execute_command(t_command *cmd, t_shell *shell);
int				exec_pipeline(t_command *head, t_shell *shell);
char			*find_executable(const char *name);

// Utility Functions
void			exit_with_error(const char *msg);
int				syntax_error(const char *unexpected_token);
int				is_valid_export_token(const char *str);
int				is_valid_identifier(const char *str);
int				is_numeric(const char *str);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strndup(const char *s, size_t n);
int				ft_strcmp(const char *s1, const char *s2);

//Env utils
char			**dup_envp(char **envp);
void			free_envp(char **envp);

//signals
void			init_signals(void);
void			def_sigint(void);
void			ignore_sigint(void);

#endif
