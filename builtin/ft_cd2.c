/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sradosav <sradosav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:01:25 by sradosav          #+#    #+#             */
/*   Updated: 2025/08/04 18:04:34 by sradosav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*cd_home(t_shell *shell)
{
	char	*get_env_return;

	get_env_return = NULL;
	get_env_return = ft_getenv("HOME", shell);
	if (get_env_return && get_env_return[0] == 0)
		return (".");
	else
		return (get_env_return);
}
