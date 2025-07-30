#include "../includes/minishell.h"

void	amb_redir(char *str, t_shell *shell, t_token **tmp)
{
	char	*msg;

	msg = ft_strjoin(AMB_REDIR, str);
	if (!msg)
		ft_clean_exit(msg, shell, NULL, NULL);
	ft_putstr_fd(msg, 2);
	write(2, "\n", 1);
	free(msg);
	(*tmp)->amb_redir = true;
}

int	should_trigger_amb_redir(t_token *token, char **split)
{
	int	len;

	len = count_strings(split);
	if (token->type == FILEN && (len > 1 || len == 0))
		return (1);
	return (0);
}

void	handle_ambiguous_redir(t_shell *shell, t_token **token,
		char **expanded, char **split)
{
	amb_redir((*token)->value, shell, token);
	(*token)->amb_redir = true;
	free(*expanded);
	ft_free_str_array(split);
}

char	**create_single_token_array(char *str)
{
	char	**arr;

	arr = malloc(sizeof(char *) * 2);
	if (!arr)
		return (NULL);
	arr[0] = ft_strdup(str);
	arr[1] = NULL;
	return (arr);
}

int	process_token(t_shell *shell, t_token **tmp,
		t_token **prev, char **expanded)
{
	char	**splitted;

	splitted = NULL;
	*expanded = join_chars(split_and_expand((*tmp)->value, shell), shell);
	if (!*expanded)
		return (free((*tmp)->value), (*tmp)->value = NULL, 0);
	if ((*tmp)->to_split)
		splitted = ft_split(*expanded, ' ');
	else
		splitted = create_single_token_array(*expanded);
	if (should_trigger_amb_redir(*tmp, splitted))
		return (handle_ambiguous_redir(shell, tmp, expanded, splitted), 0);
	*tmp = insert_new_nodes(shell, *prev, *tmp, splitted);
	*prev = *tmp;
	*tmp = (*tmp)->next;
	cleanup_token(expanded, &splitted);
	return (1);
}
