/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:30:37 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/20 18:44:43 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(char **str, t_shell *shell)
{
	char	*pwd;
	char	cwd[1024];

	pwd = ft_getenv("PWD", shell);
	shell->exit_status = 1;
	if (!str[1] || str[2])
	{
		if (!str[1])
			ft_putstr_fd("minishell: cd: missing argument\n", 2);
		else
			ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (chdir(str[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	update_or_add("OLDPWD", pwd, shell, 0);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_or_add("PWD", cwd, shell, 0);
	return (0);
}
