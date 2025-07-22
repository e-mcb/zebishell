/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:35 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 22:06:08 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_envsize(t_envvar *var)
{
	int		i;

	i = 0;
	while (var)
	{
		var = var->next;
		i++;
	}
	return (i);
}

void	env_list_to_arr(t_shell *shell)
{
	t_envvar	*tmp;
	int			lst_size;
	int			i;

	i = 0;
	tmp = shell->env;
	lst_size = ft_envsize(shell->env);
	if (shell->env_arr)
		ft_free_str_array(shell->env_arr);
	shell->env_arr = malloc(sizeof(char *) * (lst_size + 1));
	if (!shell->env_arr)
		ft_clean_exit(NULL, shell, NULL, NULL);
	while (tmp)
	{
		shell->env_arr[i] = ft_strdup(tmp->var);
		if (!shell->env_arr[i])
			ft_clean_exit(NULL, shell, NULL, NULL);
		i++;
		tmp = tmp->next;
	}
	shell->env_arr[i] = NULL;
}
