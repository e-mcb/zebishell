/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:45:52 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/16 00:05:11 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_child_heredoc(const t_token *token, int *pipefd)
{
	char	*input;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	while (1)
	{
		input = readline("> ");
		if (input == NULL)
		{
			ft_putstr_fd(EOF_HEREDOC, 2);
			ft_putstr_fd(token->value, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(input, token->value) == 0)
		{
			free(input);
			break ;
		}
		write(pipefd[1], input, strlen(input));
		write(pipefd[1], "\n", 1);
		free(input);
	}
	exit((close(pipefd[1]), 0));
}

static char	*read_heredoc_output(int *pipefd)
{
	char	*line;
	char	*finale;
	char	*tmp;

	line = NULL;
	finale = NULL;
	tmp = NULL;
	close(pipefd[1]);
	line = get_next_line(pipefd[0]);
	while (line != NULL)
	{
		if (finale != NULL)
		{
			tmp = finale;
			finale = ft_strjoin(finale, line);
			free(tmp);
		}
		else
			finale = ft_strdup(line);
		if (finale == NULL)
			return (free(line), NULL);
		free(line);
		line = get_next_line(pipefd[0]);
	}
	return (close(pipefd[0]), finale);
}

char	*do_heredoc(const t_token *token, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	char	*result;

	result = NULL;
	safe_pipe(pipefd, shell);
	signal(SIGINT, SIG_IGN);
	pid = safe_fork(shell);
	if (pid == 0)
		handle_child_heredoc(token, pipefd);
	else
	{
		result = read_heredoc_output(pipefd);
		wait_for_heredoc_to_exit(pid, shell);
		signal(SIGINT, sigint_handler);
	}
	return (result);
}
