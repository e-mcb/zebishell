/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_refine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:12:13 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/25 22:58:13 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	second_refine_token_type(t_token *token, t_shell *shell)
{
	t_token	*t;
	bool	has_cmd;
	(void) shell;

	has_cmd = false;
	t = token;
	while (t)
	{
		if (t->type == PIPE)
		{
			has_cmd = false;
			t = t->next;
		}
		if (t->type == WORD && !has_cmd)
		{
			t->type = CMD;
			has_cmd = true;
		}
		if (t->type == WORD && has_cmd)
			t->type = ARG;
		t = t->next;
	}
}
