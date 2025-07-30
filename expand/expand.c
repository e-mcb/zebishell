/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:32:53 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/30 21:59:04 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*strchr_twice(const char *str, char ch)
{
	int			count;
	const char	*first;

	count = 0;
	first = NULL;
	while (*str)
	{
		if (*str == ch)
		{
			count++;
			if (count == 1)
				first = str;
			else if (count == 2)
				return ((char *)first);
		}
		str++;
	}
	return (NULL);
}

int	should_skip_token(t_token *token)
{
	if (token->value == NULL)
		return (0);
	if (token->value[0] == 0)
		return (1);
	return (0);
}

void	check_quotes_and_set_split(t_token *token)
{
	if (strchr_twice(token->value, '"'))
		token->to_split = false;
	if (strchr_twice(token->value, '\''))
		token->to_split = false;
}

void	thanks_norminette(t_token **prev, char **expanded, int *skip)
{
	*prev = NULL;
	*expanded = NULL;
	*skip = 0;
}

void	expand(t_shell *shell)
{
	t_token	*tmp;
	t_token	*prev;
	char	*expanded;
	int		skip;

	tmp = shell->token;
	thanks_norminette(&prev, &expanded, &skip);
	while (tmp)
	{
		check_quotes_and_set_split(tmp);
		if (should_skip_token(tmp))
		{
			prev = tmp;
			tmp = tmp->next;
			continue ;
		}
		if (tmp->type != LIMITER)
			skip = process_token(shell, &tmp, &prev, &expanded);
		else
			skip = 0;
		if (skip)
			continue ;
		prev = tmp;
		tmp = tmp->next;
	}
}
