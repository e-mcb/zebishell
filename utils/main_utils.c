/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:46:47 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/15 21:19:46 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_set_shlvl(t_shell *shell, char *shlvl_str)
{
	int		value;
	char	*new_value;

	value = ft_atoi(shlvl_str);
	if (value >= 999)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		new_value = ft_itoa(value + 1);
		ft_putstr_fd(new_value, 2);
		free(new_value);
		ft_putstr_fd(") too high, resetting to 1\n", 2);
		value = 1;
	}
	else if (value < 0)
		value = 0;
	else if (value >= 0 && value < 999)
		value++;
	new_value = ft_itoa(value);
	update_or_add("SHLVL", new_value, shell, 1);
	free(new_value);
}

void	env_min(t_shell *shell)
{
	char	cwd[1024];
	char	*joined;

	if (getcwd(cwd, sizeof(cwd)) != 0)
	{
		joined = ft_strjoin("PWD=", cwd);
		if (!joined)
			ft_clean_exit(0, shell, 0, 0);
		add_env_var(&(shell->env), joined, 1, shell);
		free(joined);
	}
	else
	{
		perror("cwd init");
		ft_clean_exit(0, shell, 0, 0);
	}
	add_env_var(&(shell->env), PATH, 0, shell);
	add_env_var(&(shell->env), "OLDPWD", 1, shell);
	add_env_var(&(shell->env), "SHLVL=0", 1, shell);
}
