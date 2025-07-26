/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinexpanded.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:27:36 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/26 01:55:00 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_free_str_array_join(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	array_len(char **str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		len += ft_strlen(str[i]);
		i++;
	}
	return (len);
}

char	*join_chars(char **str, t_shell *shell)
{
	int		i;
	int		j;
	int		k;
	char	*expanded;
	
	if (!str)
		return (ft_free_str_array_join(str), NULL);
	if (str[0] == NULL)
		return (ft_free_str_array_join(str), NULL);
	expanded = malloc(sizeof(char) * (array_len(str) + 1));
	expanded[0] = 0;
	if (!expanded)
		ft_clean_exit(NULL, shell, NULL, NULL);
	i = 0;
	k = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			expanded[k] = str[i][j];
			j++;
			k++;
		}
		i++;
	}
	if (i == 0 && j == 0)
		expanded = NULL;
	else
		expanded[k] = '\0';
	return (ft_free_str_array_join(str), expanded);
}
