/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:32:36 by sradosav          #+#    #+#             */
/*   Updated: 2025/06/22 16:48:45 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(int fd_out, int exec_size)
{
	char	cwd[1024];

	if (exec_size > 1)
		fd_out = 1;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, fd_out);
		ft_putstr_fd("\n", fd_out);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
