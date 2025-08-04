/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_new_nodes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:36:56 by mzutter           #+#    #+#             */
/*   Updated: 2025/08/04 19:30:34 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_node_list(t_token **head, t_token **tail, int *i)
{
	*i = -1;
	*head = NULL;
	*tail = NULL;
}

void	create_and_link_node(char *str, t_token *current,
		t_shell *shell, t_token **tail)
{
	t_token	*new_node;

	new_node = create_token(str, shell);
	if (!new_node)
		ft_clean_exit(NULL, shell, NULL, NULL);
	new_node->type = current->type;
	new_node->in_quotes_hdoc = current->in_quotes_hdoc;
	if (!*tail)
		*tail = new_node;
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

void	create_empty_node(t_token *current, t_shell *shell,
		t_token **head, t_token **tail)
{
	create_and_link_node(NULL, current, shell, tail);
	*head = *tail;
}

t_token	*insert_new_nodes(t_shell *shell, t_token *prev,
		t_token *current, char **splitted)
{
	t_token	*head;
	t_token	*tail;
	int		i;

	init_node_list(&head, &tail, &i);
	if (!splitted || !splitted[0])
		create_empty_node(current, shell, &head, &tail);
	else
	{
		while (splitted[++i])
		{
			create_and_link_node(splitted[i], current, shell, &tail);
			if (!head)
				head = tail;
		}
	}
	if (tail)
		tail->next = current->next;
	if (prev)
		prev->next = head;
	else
		shell->token = head;
	return (free(current->value), free(current), tail);
}
