/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:31:57 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/20 18:45:40 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit_argcheck(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (42);
	else
		return (0);
}

int	ft_is_number(const char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (!ft_exit_argcheck(*str))
			return (0);
		str++;
	}
	return (1);
}

void	free_before_exit(t_shell *shell, void *ptr_a, void *ptr_b)
{
	(void)ptr_a;
	(void)ptr_b;
	if (shell)
	{
		if (shell->env)
			free_env_list(&(shell->env));
		if (shell->env_arr)
			ft_free_str_array(shell->env_arr);
		if (shell->splitted)
			ft_free_str_array(shell->splitted);
		if (shell->token)
			free_list(&(shell->token));
		if (shell->exec)
			free_exec_list(&(shell->exec));
		free(shell);
	}
}

static void	handle_exit_error(char *arg, t_shell *shell, int exec_size)
{
	if (exec_size == 1)
		ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_before_exit(shell, NULL, NULL);
	exit(2);
}

int	ft_exit(char **arr, t_shell *shell, int exec_size)
{
	long long	exit_status;

	if (!arr[1])
	{
		free_before_exit(shell, NULL, NULL);
		if (exec_size == 1)
			ft_putstr_fd("exit\n", 1);
		exit(0);
	}
	if (arr[2])
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	if (!ft_is_number(arr[1]) || is_out_of_range(arr[1]))
		handle_exit_error(arr[1], shell, exec_size);
	exit_status = ft_atoll(arr[1]) % 256;
	if (exit_status < 0)
		exit_status += 256;
	free_before_exit(shell, NULL, NULL);
	if (exec_size == 1)
		ft_putstr_fd("exit\n", 2);
	exit(exit_status);
	return (0);
}
