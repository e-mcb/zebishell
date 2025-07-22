/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:31:52 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/20 16:04:00 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(char **str, t_shell *shell, int exec_size, int fd_out)
{
	t_envvar	*env_copy;

	if (exec_size > 1)
		fd_out = 1;
	if (count_strings(str) > 1)
	{
		ft_putstr_fd("minishell: env: too many arguments or options\n", 2);
		return (1);
	}
	else
	{
		env_copy = shell->env;
		while (env_copy)
		{
			if (env_copy->exported == 1 && ft_strchr(env_copy->var, '='))
			{
				ft_putstr_fd(env_copy->var, fd_out);
				ft_putstr_fd("\n", fd_out);
			}
			env_copy = env_copy->next;
		}
		return (0);
	}
}
