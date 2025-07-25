/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:30:37 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/26 00:41:28 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(char **str, t_shell *shell)
{
	char 	*pwd_env;
	char	*temp;
	char	*temp2;
	char	cwd[1024];

	pwd_env = ft_getenv("PWD", shell);
	if (pwd_env)
		pwd_env = ft_strdup(pwd_env);
	else
		pwd_env = ft_strdup("");
	if (!pwd_env)
		ft_clean_exit(NULL, shell, NULL, NULL);
	
	if (!str[1] || str[2])
	{
		if (!str[1])
			ft_putstr_fd("minishell: cd: missing argument\n", 2); //ici retourner au home
		else
			ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		free(pwd_env);
		return (1);
	}
	if (chdir(str[1]) != 0)
	{
		perror("minishell: cd");
		free (pwd_env);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (shell->pwd)
			free(shell->pwd);
		shell->pwd = ft_strdup(cwd);
		if (!shell->pwd)
			ft_clean_exit(NULL, shell, NULL, NULL);
		update_or_add("PWD", cwd, shell, 1);
	}
	else
	{
		ft_putstr_fd("erreur de détermination du répertoire actuel : getcwd : ne peut accéder aux répertoires parents  ", 2);
		perror("minishell: cd");
		temp = ft_strjoin(shell->pwd, "/");
		if (!temp)
			ft_clean_exit(NULL, shell, NULL, NULL);
		temp2 = ft_strjoin(temp, str[1]);
		if (!temp2)
			ft_clean_exit(NULL, shell, NULL, NULL);
		if (shell->pwd)
			free(shell->pwd);
		shell->pwd = temp2;
		update_or_add("PWD", shell->pwd, shell, 1);
	}
	update_or_add("OLDPWD", pwd_env, shell, 1);
	free (pwd_env);
	return (0);
}
