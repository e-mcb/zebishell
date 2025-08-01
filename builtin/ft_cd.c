/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:30:37 by sradosav          #+#    #+#             */
/*   Updated: 2025/08/01 01:59:47 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_cd_errors(char *path, char **str, char *pwd_env)
{
	if (!path)
	{
		if (str[1] && ft_strcmp(str[1], "-") == 0)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		else
			return (free(pwd_env), ft_putstr_fd("minishell cd: HOME not set\n", 2), 1);
		return (1);
	}
	if (count_strings(str) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		free(pwd_env);
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": no such file or directory\n", 2);
		free (pwd_env);
		return (1);
	}
	return (0);
}

static void	update_pwd(char *cwd, t_shell *shell)
{
	if (shell->pwd)
		free(shell->pwd);
	shell->pwd = ft_strdup(cwd);
	if (!shell->pwd)
		ft_clean_exit(NULL, shell, NULL, NULL);
	update_or_add("PWD", cwd, shell, 1);
}

static void	handle_broken_path(char *path, t_shell *shell)
{
	char	*temp;
	char	*temp2;

	ft_putstr_fd("erreur de détermination du répertoire actuel : ", 2);
	ft_putstr_fd("getcwd : ne peut accéder aux répertoires parents ", 2);
	ft_putstr_fd(": no such file or directory\n", 2);
	temp = ft_strjoin(shell->pwd, "/");
	if (!temp)
		ft_clean_exit(NULL, shell, NULL, NULL);
	temp2 = ft_strjoin(temp, path);
	if (!temp2)
		ft_clean_exit(NULL, shell, NULL, NULL);
	if (shell->pwd)
		free(shell->pwd);
	shell->pwd = temp2;
	update_or_add("PWD", shell->pwd, shell, 1);
}

static int	define_path(char **str, char **path, t_shell *shell, char *pwd_env)
{
	if (!str[1] || (str[1] && ft_strcmp(str[1], "~") == 0))
		*path = ft_getenv("HOME", shell);
	else if (str[1] && ft_strcmp(str[1], "-") == 0)
	{
		*path = ft_getenv("OLDPWD", shell);
		if (!*path || !**path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			free(pwd_env);
			return (1);
		}
		ft_putstr_fd(*path, 1);
		ft_putchar_fd('\n', 1);
	}
	else
		*path = str[1];
	return (0);
}

int	ft_cd(char **str, t_shell *shell)
{
	char	*pwd_env;
	char	cwd[1024];
	char	*path;

	path = NULL;
	pwd_env = NULL;
	pwd_env = ft_getenv("PWD", shell);
	if (pwd_env)
		pwd_env = ft_strdup(pwd_env);
	else
		pwd_env = ft_strdup("");
	if (!pwd_env)
		ft_clean_exit(NULL, shell, NULL, NULL);
	if (define_path(str, &path, shell, pwd_env) == 1)
		return (1);
	if (handle_cd_errors(path, str, pwd_env))
		return (1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_pwd(cwd, shell);
	else
		handle_broken_path(path, shell);
	update_or_add("OLDPWD", pwd_env, shell, 1);
	free (pwd_env);
	return (0);
}
