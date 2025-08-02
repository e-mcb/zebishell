/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:56:11 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/23 23:45:05 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_execsize(t_exec *exec)
{
	int		i;

	i = 0;
	while (exec)
	{
		exec = exec->next;
		i++;
	}
	return (i);
}

void	exec_loop(t_shell *shell)
{
	t_exec	*tmp;
	int		pipe_fd[2];
	int		prev_fd_in;
	pid_t	last_pid;
	int		ret;

	ret = 0;
	tmp = shell->exec;
	prev_fd_in = STDIN_FILENO;
	ret = should_run_single_builtin(shell, tmp);
	if (ret)
		return ;
	last_pid = execute_all_commands(shell, tmp, pipe_fd, prev_fd_in);
	if (prev_fd_in != STDIN_FILENO)
		close(prev_fd_in);
	if (last_pid < 0)
		wait_for_remaining_children();
	else
		wait_for_children_to_exit(shell, last_pid);
	signal(SIGINT, sigint_handler);
}
