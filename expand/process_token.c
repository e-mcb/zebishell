/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:37:04 by mzutter           #+#    #+#             */
/*   Updated: 2025/08/04 19:29:42 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	handle_ambiguous_redir(t_shell *shell, t_token **token,
		char **expanded, char **split)
{
	int	len;

	len = 0;
	if (split)
		len = count_strings(split);
	if ((*token)->type == FILEN && (!split || (len != 1)))
	{
		amb_redir((*token)->value, shell, token);
		(*token)->amb_redir = true;
		free(*expanded);
		free((*token)->value);
		(*token)->value = NULL;
		ft_free_str_array(split);
		return (0);
	}
	if (!*expanded)
	{
		free ((*token)->value);
		(*token)->value = NULL;
	}
	return (1);
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
		return (handle_ambiguous_redir(shell, tmp,
				expanded, splitted), 0);
	if ((*tmp)->to_split)
		splitted = ft_split(*expanded, ' ');
	else
		splitted = create_single_token_array(*expanded);
	if (!handle_ambiguous_redir(shell, tmp, expanded, splitted))
		return (0);
	*tmp = insert_new_nodes(shell, *prev, *tmp, splitted);
	*prev = *tmp;
	*tmp = (*tmp)->next;
	cleanup_token(expanded, &splitted);
	return (1);
}
