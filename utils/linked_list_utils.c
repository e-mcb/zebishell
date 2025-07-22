/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:43:06 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/04 00:51:01 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *str, t_shell *shell)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		ft_clean_exit(NULL, shell, NULL, NULL);
	new_token->value = ft_strdup(str);
	new_token->type = -1;
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	new_token->in_quotes = false;
	return (new_token);
}

int	add_token(t_shell *shell, char *str, t_token_type type, int rank)
{
	t_token	*new_token;
	t_token	*current_token;

	new_token = create_token(str, shell);
	if (!new_token)
		ft_clean_exit(NULL, shell, NULL, NULL);
	new_token->type = type;
	new_token->rank = rank;
	if (shell->token == NULL)
	{
		shell->token = new_token;
		return (0);
	}
	current_token = shell->token;
	while (current_token->next != NULL)
		current_token = current_token->next;
	current_token->next = new_token;
	return (0);
}

void	cleanup_token(char **expanded, char ***splitted)
{
	if (*splitted)
	{
		ft_free_str_array(*splitted);
		*splitted = NULL;
	}
	if (*expanded)
	{
		free(*expanded);
		*expanded = NULL;
	}
}

t_token	*new_token_append(t_token *head, char *str,
	t_token_type type, t_shell *shell)
{
	t_token	*new_node;
	t_token	*last;

	new_node = create_token(str, shell);
	new_node->type = type;
	if (!head)
		return (new_node);
	last = head;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return (head);
}
