#include "../include/minishell.h"

// Helper function to set file values safely
int set_redirection_file(char **dest, char *src)
{
	if (*dest)
		free(*dest);
	*dest = ft_strdup(src);
	if (!(*dest))
	{
		perror("strdup");
		return (-1);
	}
	return (0);
}

static int store_redir(char **dest, const char *raw)
{
    // strip any leading/trailing quotes
    char *clean = remove_surrounding_quotes(raw);
    if (!clean)
        return -1;
    if (set_redirection_file(dest, clean) == -1)
    {
        free(clean);
        return -1;
    }
    free(clean);
    return 0;
}

int parse_input_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;
    if (store_redir(&cmd->input_file, raw) == -1)
        return -1;
    *cur = (*cur)->next->next;
    return 0;
}

int parse_output_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;

    // -- create it right away, so ">f1 >f2" still makes f1 --
    {
      int fd = open(raw,
                    O_WRONLY|O_CREAT|(cmd->append_mode?O_APPEND:O_TRUNC),
                    0644);
      if (fd >= 0) close(fd);
      // you might want to perror(raw) if fd<0, but bash continues on those errors
    }

    // -- now remember only the _last_ one for the exec-time dup2() --
    free(cmd->output_file);
    cmd->output_file = strdup(raw);
    cmd->append_mode = 0;
    *cur = (*cur)->next->next;
    return 0;
}

int parse_append_redirection(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;

    // 1) eagerly create/truncate the file exactly as bash does,
    //    so “>>f1 >>f2” still creates f1 even though you’ll only
    //    open f2 for the final dup2().
    {
        int fd = open(raw, O_WRONLY|O_CREAT|O_APPEND, 0644);
        if (fd >= 0)
            close(fd);
        // note: bash does *not* abort on open errors here; it will
        // emit an error later when you actually try to use it
    }

    // 2) now record the *last* filename in your cmd struct, and mark append
    free(cmd->output_file);
    cmd->output_file = strdup(raw);
    if (!cmd->output_file)
    {
        perror("strdup");
        return -1;
    }
    cmd->append_mode = 1;

    // 3) advance past the “>>” and its argument
    *cur = (*cur)->next->next;
    return 0;
}

int parse_heredoc(t_command *cmd, t_token **cur)
{
    char *raw;
    if (check_next_token(*cur, &raw) == -1)
        return -1;

    // strip quotes for the *delimiter* comparison
    char *clean = remove_surrounding_quotes(raw);
    if (!clean) return -1;

    // if unmatched, bash silently ignores
    if (!has_unmatched_quotes(raw))
    {
        if (set_redirection_file(&cmd->heredoc_delimiter, clean) == -1)
        {
            free(clean);
            return -1;
        }
        cmd->has_heredoc    = 1;
        cmd->expand_heredoc = (raw[0] != '\'' && raw[0] != '"');
    }
    free(clean);

    *cur = (*cur)->next->next;
    return 0;
}

int parse_redirections(t_command *cmd, t_token **tokens)
{
    if (!*tokens)
        return -1;

    if ((*tokens)->type == REDIR_OUT)
        return parse_output_redirection(cmd, tokens);
    else if ((*tokens)->type == REDIR_APPEND)
        return parse_append_redirection(cmd, tokens);
    else if ((*tokens)->type == REDIR_IN)
        return parse_input_redirection(cmd, tokens);
    else if ((*tokens)->type == HEREDOC)
        return parse_heredoc(cmd, tokens);

    return 0;
}

