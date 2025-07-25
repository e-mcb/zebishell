/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:32:53 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/26 00:25:07 by mzutter          ###   ########.fr       */
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

static void	thanks_norminette(t_token **head, t_token **tail, int *i)
{
	*head = NULL;
	*tail = NULL;
	*i = -1;
}

t_token	*insert_new_nodes(t_shell *shell, t_token *prev,
		t_token *current, char **splitted)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_node;
	int		i;

	thanks_norminette(&head, &tail, &i);
	while (splitted[++i])
	{
		new_node = create_token(splitted[i], shell);
		if (!new_node)
			ft_clean_exit(NULL, shell, NULL, NULL);
		new_node->type = current->type;
		new_node->in_quotes = current->in_quotes_hdoc;
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
	}
	if (tail)
		tail->next = current->next;
	if (prev)
		prev->next = head;
	else
		shell->token = head;
	return (free(current->value), free(current), tail);
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

int	process_token(t_shell *shell, t_token **tmp, t_token **prev,
			char **expanded)
{
	char	**splitted;

	splitted = NULL;
	*expanded = join_chars(split_and_expand((*tmp)->value, shell), shell);
	if (*expanded)
	{
		if (((*tmp)->to_split))
			splitted = ft_split(*expanded, ' ');
			// protection et gestion de l'ambiguous si echo > $idiot
		else
			splitted = create_single_token_array(*expanded);
		if (((*tmp)->type == FILEN && count_strings(splitted) > 1)
			|| ((*tmp)->type == FILEN && count_strings(splitted) == 0))
		{
			amb_redir((*tmp)->value, shell, tmp);
			(*tmp)->amb_redir = true;
			return (free(*expanded), ft_free_str_array(splitted), 0);
		}
		*tmp = insert_new_nodes(shell, *prev, *tmp, splitted);
		*prev = *tmp;
		*tmp = (*tmp)->next;
	}
	else
	{
		if ((*tmp)->type == FILEN && !*expanded)
		{
			amb_redir((*tmp)->value, shell, tmp);
			(*tmp)->amb_redir = true;
		}
		free((*tmp)->value);
		(*tmp)->value = NULL;
		return (0);
	}
	return (cleanup_token(expanded, &splitted), 1);
}

char *strchr_twice(const char *str, char ch) {
    int count = 0;
    const char *first = NULL;

    while (*str) {
        if (*str == ch) {
            count++;
            if (count == 1)
                first = str;
            else if (count == 2)
                return (char *)first; // return pointer to first occurrence
        }
        str++;
    }
	//a changer pour prendre en compte les nested quotes

    return NULL; // return NULL if less than 2 occurrences found
}


void	expand(t_shell *shell)
{
	t_token	*tmp;
	t_token	*prev;
	char	*expanded;
	int		skip;

	tmp = shell->token;
	prev = NULL;
	expanded = NULL;
	skip = 0;
	while (tmp)
	{
		if (strchr_twice(tmp->value, '"') || strchr_twice(tmp->value, '\''))
			tmp->to_split = false;
		if (tmp->value && tmp->value[0] == 0)
		{
			prev = tmp;
			tmp = tmp->next;
			continue ;
		}
		skip = process_token(shell, &tmp, &prev, &expanded);
		if (skip)
			continue ;
		prev = tmp;
		tmp = tmp->next;
	}
}
