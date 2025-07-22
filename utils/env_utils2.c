/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:46:44 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/06 18:55:20 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_envvar	*copy_env_list(t_envvar *env, t_shell *shell)
{
	t_envvar	*copy;
	t_envvar	*tail;
	t_envvar	*new_node;

	copy = NULL;
	tail = NULL;
	while (env)
	{
		new_node = malloc(sizeof(t_envvar));
		if (!new_node)
			return (free_env_list(&copy), ft_clean_exit(0, shell, 0, 0), NULL);
		new_node->var = ft_strdup(env->var);
		if (!new_node->var)
			return (free(new_node), free_env_list(&copy),
				ft_clean_exit(NULL, shell, NULL, NULL), NULL);
		new_node->next = NULL;
		new_node->exported = env->exported;
		if (!copy)
			copy = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		env = env->next;
	}
	return (copy);
}

void	ft_sort_env_list(t_envvar *head)
{
	int			swapped;
	t_envvar	*ptr;
	char		*tmp;

	if (!head)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr = head;
		while (ptr->next)
		{
			if (ft_strcmp(ptr->var, ptr->next->var) > 0)
			{
				tmp = ptr->var;
				ptr->var = ptr->next->var;
				ptr->next->var = tmp;
				swapped = 1;
			}
			ptr = ptr->next;
		}
	}
}

int	envvar_match(char *env_var, char *var, size_t len, char *full_var)
{
	return (ft_strncmp(env_var, full_var, len) == 0
		|| ft_strcmp(env_var, var) == 0);
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
