/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:56:11 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/20 17:49:12 by mzutter          ###   ########.fr       */
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

void	wait_for_children_to_exit(t_shell *shell, pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		sig;
	int		newline;
	int		quit;

	newline = 0;
	quit = 0;
	wpid = wait(&status);
	while (wpid > 0)
	{
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			shell->exit_status = 128 + sig;
			if (sig == SIGQUIT)
				quit = 1;
			if (sig == SIGINT)
				newline = 1;
		}
		else if (wpid == last_pid && WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		wpid = wait(&status);
	}
	if (newline)
		write(1, "\n", 1);
	if (quit)
		write(1, "Quit\n", 5);
}

void	wait_for_remaining_children(void)
{
	int		status;
	pid_t	wpid;
	int		sig;

	wpid = wait(&status);
	while (wpid > 0)
	{
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(1, "Quit\n", 5);
			else if (sig == SIGINT)
				write(1, "\n", 1);
		}
		wpid = wait(&status);
	}
}

void	wait_for_heredoc_to_exit(pid_t pid, t_shell *shell)
{
	int		status;
	int		sig;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			shell->exit_status = 128 + sig;
			g_signal = SIGINT;
		}
	}
}

void	exec_loop(t_shell *shell)
{
	t_exec	*tmp;
	int		pipe_fd[2];
	int		prev_fd_in;
	pid_t	last_pid;

	tmp = shell->exec;
	prev_fd_in = STDIN_FILENO;
	if (should_run_single_builtin(shell, tmp))
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
