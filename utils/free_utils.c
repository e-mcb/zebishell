/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:46:59 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/15 19:47:01 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_list(t_token **head)
{
	t_token	*current;
	t_token	*next;

	current = *head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*head = NULL;
}

void	ft_free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL && arr[i][0] != 0)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_free_str_array_exec(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_exec_list(t_exec **exec)
{
	t_exec	*next;

	if (!exec || !*exec)
		return ;
	while (*exec)
	{
		next = (*exec)->next;
		if ((*exec)->arr)
			ft_free_str_array_exec((*exec)->arr);
		if ((*exec)->heredoc)
			free((*exec)->heredoc);
		if ((*exec)->fd_in != STDIN_FILENO && (*exec)->fd_in > -1)
			close((*exec)->fd_in);
		if ((*exec)->fd_out != STDOUT_FILENO && (*exec)->fd_out > -1)
			close((*exec)->fd_out);
		free(*exec);
		*exec = next;
	}
	*exec = NULL;
}
