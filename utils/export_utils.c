/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:32:04 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/23 20:22:37 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_write_export(char *str, int fd_out)
{
	int	i;

	write(fd_out, "declare -x ", 11);
	i = 0;
	while (str[i] != 0 && str[i] != '=')
	{
		write(fd_out, &str[i], 1);
		i++;
	}
	if (str[i] == '=')
	{
		write(fd_out, &str[i], 1);
		write(fd_out, "\"", 1);
		i++;
		while (str[i] != 0)
		{
			if (str[i] == '"')
				write(fd_out, "\"", 1);
			write(fd_out, &str[i], 1);
			i++;
		}
		write(fd_out, "\"", 1);
	}
	write(fd_out, "\n", 1);
}

int	add_env_var_without_value(t_envvar **head,
	char *str, int exported, t_shell *shell)
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

void	update_or_add_without_value(char *var, t_shell *shell)
{
	int		exists;

	exists = env_var_exists(var, shell);
	if (exists == 1)
		export_existing_var(var, shell);
	else
		add_env_var_without_value(&(shell->env), var, 1, shell);
	return ;
}
