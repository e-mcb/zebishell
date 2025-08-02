/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 23:29:25 by mzutter           #+#    #+#             */
/*   Updated: 2025/08/01 01:24:23 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_msg(t_token *token)
{
	if (token->type == IN)
		ft_putstr_fd("'<'\n", 2);
	else if (token->type == OUT)
		ft_putstr_fd("'>'\n", 2);
	else if (token->type == APPEND)
		ft_putstr_fd("'>>'\n", 2);
	else if (token->type == HDOC)
		ft_putstr_fd("'<<'\n", 2);
}

int	token_error(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	if (tmp && tmp->type == PIPE)
		return (ft_putstr_fd("syntax error near token '|'\n", 2), 1);
	while (tmp)
	{
		if (tmp->type == OUT && tmp->next && tmp->next->type == PIPE)
			return (ft_putstr_fd(NOCLOBBER, 2), 1);
		if ((tmp->type == IN || tmp->type == APPEND || tmp->type == HDOC)
			&& tmp->next && tmp->next->type == PIPE)
			return (ft_putstr_fd("syntax error near token '|'\n", 2), 1);
		if ((is_redir(tmp) || tmp->type == HDOC) && tmp->next
			&& (is_redir(tmp->next) || tmp->next->type == HDOC))
		{
			ft_putstr_fd(SUCCESSIVE_OPERATORS, 2);
			print_msg(tmp->next);
			return (1);
		}
		if (tmp->type == PIPE && tmp->next && tmp->next->type == PIPE)
			return (ft_putstr_fd(SUCCESSIVE_PIPES, 2), 1);
		tmp = tmp->next;
	}
	return (0);
}

