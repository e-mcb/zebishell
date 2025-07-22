/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refine_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sradosav <sradosav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:12:13 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/04 02:19:17 by sradosav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	is_str_digit(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] < '0' || str[i] > '9')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

bool	is_redir(t_token *t)
{
	return (t && (t->type == IN || t->type == OUT || t->type == APPEND));
}

static bool	is_fd_case(t_token *t)
{
	return (t->next && (t->next->type == IN || t->next->type == OUT
			|| t->next->type == APPEND || t->next->type == HDOC)
		&& t->rank == t->next->rank && is_str_digit(t->value));
}

static void	set_word_type(t_token *t, t_token *p)
{
	if (p && is_redir(p))
		t->type = FILEN;
	else if (p && p->type == HDOC)
		t->type = LIMITER;
	else if (is_fd_case(t))
		t->type = FD;
	else
		t->type = WORD;
}

void	refine_token_type(t_token *token)
{
	t_token	*t;
	t_token	*p;

	t = token;
	p = NULL;
	while (t)
	{
		if (t->type == WORD)
			set_word_type(t, p);
		p = t;
		t = t->next;
	}
}
