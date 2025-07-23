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

void	ft_print_export(t_shell *shell, int fd_out, int exec_size)
{
	t_envvar	*env_copy;
	t_envvar	*iter;

	(void) exec_size;
	env_copy = copy_env_list(shell->env, shell);
	ft_sort_env_list(env_copy);
	iter = env_copy;
	while (iter)
	{
		if (iter->exported == 1 && ft_strncmp("_=", iter->var, 2) != 0)
			ft_write_export(iter->var, fd_out);
		iter = iter->next;
	}
	free_env_list(&env_copy);
}

void	export_existing_var(char *var, t_shell *shell)
{
	char		*full_var;
	t_envvar	*copy_env;
	size_t		len;

	full_var = ft_strjoin(var, "=");
	if (!full_var)
		ft_clean_exit(NULL, shell, NULL, NULL);
	len = ft_strlen(full_var);
	copy_env = shell->env;
	while (copy_env)
	{
		if (envvar_match(copy_env->var, var, len, full_var))
		{
			copy_env->exported = 1;
			free(full_var);
			return ;
		}
		copy_env = copy_env->next;
	}
	free(full_var);
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

void	handle_valid_export(char *str, t_shell *shell)
{
	int		var_len;
	char	*var;
	char	*value;

	var_len = 0;
	while (str[var_len] && str[var_len] != '=')
		var_len++;
	var = ft_strndup(str, var_len);
	if (!var)
		ft_clean_exit(NULL, shell, NULL, NULL);
	if (ft_strchr(str, '='))
	{
		value = ft_strchr(str, '=') + 1;
		update_or_add(var, value, shell, 1);
	}
	else
		update_or_add_without_value(var, shell);
	free(var);
}

int	ft_export_vars(char **str, t_shell *shell)
{
	int		i;

	i = 1;
	while (str[i])
	{
		if (!is_valid_identifier(str[i]))
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(str[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (1);
		}
		else
			handle_valid_export(str[i], shell);
		i++;
	}
	return (0);
}

int	ft_export(char **str, t_shell *shell, int exec_size, int fd_out)
{
	if (!str[1])
	{
		if (exec_size > 1)
			fd_out = 1;
		ft_print_export(shell, fd_out, exec_size);
		return (0);
	}
	else if (exec_size > 1 && !is_valid_identifier(str[1]))
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(str[1], 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (1);
	}
	else if (exec_size == 1)
		return (ft_export_vars(str, shell));
	return (0);
}
