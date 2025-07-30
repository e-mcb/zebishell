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

int	should_return_null(char **str)
{
	if (!str)
		return (1);
	if (str[0] == NULL)
		return (1);
	return (0);
}

char	*alloc_expanded_string(char **str, t_shell *shell)
{
	char	*expanded;
	int		len;

	len = array_len(str) + 1;
	expanded = malloc(sizeof(char) * len);
	if (!expanded)
		ft_clean_exit(NULL, shell, NULL, NULL);
	expanded[0] = 0;
	return (expanded);
}

void	copy_chars(char **str, char *expanded)
{
	int	i;
	int	j;
	int	k;

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
	expanded[k] = '\0';
}

char	*join_chars(char **str, t_shell *shell)
{
	char	*expanded;

	if (should_return_null(str))
	{
		ft_free_str_array_join(str);
		return (NULL);
	}
	expanded = alloc_expanded_string(str, shell);
	copy_chars(str, expanded);
	ft_free_str_array_join(str);
	return (expanded);
}
