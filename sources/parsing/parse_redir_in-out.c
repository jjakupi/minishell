#include "../include/minishell.h"

int parse_input_redirection(t_command *cmd, t_token **current)
{
	if (!*current || (*current)->type != REDIR_IN)
		return (-1);
	if (!(*current)->next || (*current)->next->type != WORD)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			(*current)->next ? (*current)->next->value : "newline");
		return (-1);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup((*current)->next->value);
	if (!cmd->input_file)
		return (-1);
	*current = (*current)->next->next;
	return (0);
}


int parse_output_redirection(t_command *cmd, t_token **current)
{
    char *value;

    if (check_next_token(*current, &value) == -1)
        return -1;

    if (cmd->output_file)
        free(cmd->output_file);

    cmd->output_file = strdup(value);
    if (!cmd->output_file)
    {
        perror("strdup");
        return -1;
    }
    cmd->append_mode = 0;

    *current = (*current)->next->next;
    return 0;
}

