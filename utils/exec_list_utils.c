/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_list_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:37:09 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/30 21:37:10 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_exec	*init_exec(void)
{
	t_exec	*exec;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (NULL);
	exec->arr = NULL;
	exec->fd_in = 0;
	exec->fd_out = 1;
	exec->next = NULL;
	exec->heredoc = NULL;
	exec->heredoc_bool = false;
	exec->pid = -1;
	exec->amb_redir = false;
	return (exec);
}

t_exec	*new_node(t_exec *head)
{
	t_exec	*new;
	t_exec	*last;

	new = init_exec();
	if (!new)
		return (NULL);
	if (!head)
		return (new);
	last = head;
	while (last->next)
		last = last->next;
	last->next = new;
	return (new);
}
