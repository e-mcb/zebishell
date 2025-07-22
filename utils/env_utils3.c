/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:30:55 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/20 16:09:25 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env_list(t_envvar **head)
{
	t_envvar	*current;
	t_envvar	*next;

	current = *head;
	while (current)
	{
		next = current->next;
		if (current->var)
		{
			free(current->var);
			current->var = NULL;
		}
		free(current);
		current = next;
	}
	*head = NULL;
}

static t_envvar	*create_env_var(char *str, int exported, t_shell *shell)
{
	t_envvar	*node;

	node = malloc(sizeof(t_envvar));
	if (!node)
		ft_clean_exit(NULL, shell, NULL, NULL);
	node->var = ft_strdup(str);
	if (!node->var)
	{
		free(node);
		ft_clean_exit(NULL, shell, NULL, NULL);
	}
	node->exported = exported;
	node->next = NULL;
	return (node);
}

int	add_env_var(t_envvar **head, char *str, int exported, t_shell *shell)
{
	t_envvar	*new_node;
	t_envvar	*current;

	new_node = create_env_var(str, exported, shell);
	if (!new_node)
	{
		free_env_list(head);
		ft_clean_exit(NULL, shell, NULL, NULL);
	}
	if (!*head)
	{
		*head = new_node;
		return (1);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (1);
}

t_envvar	*ft_env_to_list(char **envp, t_shell *shell)
{
	t_envvar	*env;
	int			i;
	int			exported;

	if (!envp)
		return (NULL);
	env = NULL;
	i = 0;
	exported = -1;
	while (envp[i])
	{
		exported = 1;
		if (strncmp(envp[i], "_=", 2) == 0)
			exported = 1;
		add_env_var(&env, envp[i], exported, shell);
		i++;
	}
	return (env);
}
