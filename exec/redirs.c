/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:47:29 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/04 02:11:41 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_in(t_exec *exec, t_token *tmp)
{
	if (exec->fd_in > 0)
		close(exec->fd_in);
	exec->fd_in = open(tmp->next->value, O_RDONLY);
	if (exec->fd_in < 0)
	{
		perror("Error (infile)");
		return (1);
	}
	exec->heredoc_bool = false;
	return (0);
}

static int	handle_out(t_exec *exec, t_token *tmp)
{
	if (exec->fd_out > 1)
		close(exec->fd_out);
	exec->fd_out = open(tmp->next->value, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (exec->fd_out < 0)
	{
		perror("Error (outfile)");
		return (1);
	}
	return (0);
}

static int	handle_append(t_exec *exec, t_token *tmp)
{
	if (exec->fd_out > 1)
		close(exec->fd_out);
	exec->fd_out = open(tmp->next->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (exec->fd_out < 0)
	{
		perror("Error (append)");
		return (1);
	}
	return (0);
}

static int	handle_heredoc(t_exec *exec, t_token *tmp, t_shell *shell)
{
	char	*tmp_str;

	tmp_str = NULL;
	if (exec->fd_in > 0)
		close(exec->fd_in);
	exec->fd_in = 0;
	if (exec->heredoc)
		free(exec->heredoc);
	tmp->next->value = remove_quotes(tmp->next->value, shell);
	exec->heredoc = do_heredoc((const t_token *)tmp->next, shell);
	if (tmp->next->in_quotes == false)
	{
		tmp_str = exec->heredoc;
		exec->heredoc = join_chars(split_and_expand(exec->heredoc, shell),
				shell);
		free(tmp_str);
	}
	exec->heredoc_bool = true;
	exec->fd_in = 0;
	return (0);
}

t_token	*handle_redir(t_exec *exec, t_token *tmp, t_shell *shell)
{
	int	skip;

	skip = 0;
	if (tmp->type == IN)
		skip = handle_in(exec, tmp);
	else if (tmp->type == OUT)
		skip = handle_out(exec, tmp);
	else if (tmp->type == APPEND)
		skip = handle_append(exec, tmp);
	else if (tmp->type == HDOC)
		skip = handle_heredoc(exec, tmp, shell);
	if (skip)
	{
		if (exec->fd_in > 0)
			close(exec->fd_in);
		if (exec->fd_out > 1)
			close(exec->fd_out);
		tmp = skip_to_pipe(tmp);
	}
	return (tmp);
}
