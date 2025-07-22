/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:32:39 by sradosav          #+#    #+#             */
/*   Updated: 2025/06/21 00:13:02 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	delete_node(char *var_name, t_envvar **head)
{
	t_envvar	*current;
	t_envvar	*prev;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strncmp(current->var, var_name, ft_strlen(var_name)) == 0
			&& current->var[ft_strlen(var_name)] == '=')
		{
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			free(current->var);
			current->var = NULL;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **str, t_shell *shell, int exec_size)
{
	int	i;

	i = 1;
	if (exec_size <= 1)
	{
		while (str[i])
		{
			delete_node(str[i], &shell->env);
			i++;
		}
	}
	return (0);
}
