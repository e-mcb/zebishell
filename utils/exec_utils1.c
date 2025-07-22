/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:51:01 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/21 23:05:54 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_command(t_shell *shell, t_exec *tmp)
{
	int		i;
	char	*path;

	i = 2;
	if (ft_strcmp(tmp->arr[0], "minishell") == 0
		|| ft_strcmp(tmp->arr[0], "./minishell") == 0)
		path = (ft_strdup("/tmp/minishell"));
	else if (ft_strchr(tmp->arr[0], '/'))
		path = tmp->arr[0];
	else
		path = pathfinder(shell, tmp);
	if (path == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tmp->arr[0], 2);
		ft_putstr_fd(": command not found ", 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	while (++i < 1023)
		close(i);
	execve(path, tmp->arr, shell->env_arr);
	perror("direct path not found");
	exit(127);
}

pid_t	safe_fork(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_clean_exit(NULL, shell, NULL, NULL);
	return (pid);
}

int	safe_pipe(int *pipe_fd, t_shell *shell)
{
	if (pipe(pipe_fd) < 0)
		ft_clean_exit(NULL, shell, NULL, NULL);
	return (0);
}

int	is_valid_command(t_exec *tmp)
{
	return (tmp->arr != NULL && tmp->arr[0] != NULL);
}

void	handle_child_process(t_shell *shell, t_exec *tmp, int *pipe_fd)
{
	int	status;

	setup_redirection(tmp, pipe_fd, shell);
	handle_heredoc_input(tmp, pipe_fd);
	if (ft_is_builtin(tmp->arr[0]))
	{
		status = call_builtin(shell, tmp, tmp->arr[0]);
		if (ft_execsize(shell->exec) > 1)
			ft_clean_without_exit(shell);
		exit(status);
	}
	else
		execute_command(shell, tmp);
}
