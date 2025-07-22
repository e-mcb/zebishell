/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sradosav <sradosav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 21:18:02 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/17 17:53:05 by sradosav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_clean_exit(char *input, t_shell *shell,
	char *str_to_free, char **arr_to_free)
{
	if (str_to_free)
		free(str_to_free);
	if (arr_to_free)
		ft_free_str_array(arr_to_free);
	if (input)
		free(input);
	if (shell->env)
		free_env_list(&(shell->env));
	if (shell->env_arr)
		ft_free_str_array(shell->env_arr);
	if (shell->token)
		free_list(&(shell->token));
	if (shell->exec)
		free_exec_list(&(shell->exec));
	if (shell)
		free(shell);
	rl_clear_history();
	exit(1);
}

void	ft_clean_without_exit(t_shell *shell)
{
	if (shell->env)
		free_env_list(&(shell->env));
	if (shell->env_arr)
		ft_free_str_array(shell->env_arr);
	if (shell->token)
		free_list(&(shell->token));
	if (shell->exec)
		free_exec_list(&(shell->exec));
	if (shell)
		free(shell);
	rl_clear_history();
}

void	ft_end_minishell(char *input, t_shell *shell,
	char *str_to_free, char **arr_to_free)
{
	if (str_to_free)
		free(str_to_free);
	if (arr_to_free)
		ft_free_str_array(arr_to_free);
	if (input)
		free(input);
	if (shell->env)
		free_env_list(&(shell->env));
	if (shell->env_arr)
		ft_free_str_array(shell->env_arr);
	if (shell->token)
		free_list(&(shell->token));
	if (shell->exec)
		free_exec_list(&(shell->exec));
	if (shell)
		free(shell);
	rl_clear_history();
	exit(0);
}
