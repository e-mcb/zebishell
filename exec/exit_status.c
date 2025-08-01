/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:36:46 by mzutter           #+#    #+#             */
/*   Updated: 2025/08/01 00:09:07 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_signal_exit(int status,
				t_shell *shell, int *newline, int *quit)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		shell->exit_status = 128 + sig;
		if (sig == SIGQUIT)
			*quit = 1;
		if (sig == SIGINT)
			*newline = 1;
	}
}

void	wait_for_children_to_exit(t_shell *shell, pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		newline;
	int		quit;

	newline = 0;
	quit = 0;
	wpid = wait(&status);
	while (wpid > 0)
	{
		handle_signal_exit(status, shell, &newline, &quit);
		if (wpid == last_pid && WIFEXITED(status))
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

void	wait_for_heredoc_to_exit(pid_t pid, int *pipefd, t_shell *shell)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			shell->exit_status = 128 + sig;
			g_signal = SIGINT;
			close(pipefd[0]);
			close(pipefd[1]);
		}
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
}
