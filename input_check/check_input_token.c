/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 23:29:25 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/15 21:39:46 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	token_error(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == OUT && tmp->next->type == PIPE)
			return (ft_putstr_fd(NOCLOBBER, 2), 1);
		if ((tmp->type == IN || tmp->type == APPEND
				|| tmp->type == HDOC) && tmp->next->type == PIPE)
			return (ft_putstr_fd("syntax error near token '|'", 2), 1);
		if ((is_redir(tmp) || tmp->type == HDOC) && tmp->next
			&& (is_redir(tmp->next) || tmp->next->type == HDOC))
			return (ft_putstr_fd(SUCCESSIVE_OPERATORS, 2), 1);
		if (tmp->type == PIPE && tmp->next->type == PIPE)
			return (ft_putstr_fd(SUCCESSIVE_PIPES, 2), 1);
		tmp = tmp->next;
	}
	return (0);
}
