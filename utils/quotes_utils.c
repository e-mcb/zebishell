/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:46:24 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/15 19:46:25 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	remove_outer_quotes(char *str)
{
	size_t	len;
	size_t	i;
	size_t	j;

	len = strlen(str);
	i = 1;
	j = 0;
	while (i < len - 1)
	{
		str[j] = str[i];
		i++;
		j++;
	}
	str[j] = '\0';
}

static void	remove_all_quotes(char *str)
{
	size_t	len;
	size_t	i;
	size_t	j;

	len = strlen(str);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] != '\'' && str[i] != '"')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
}

void	remove_nested_quotes(char *str)
{
	int		fully_quoted;
	size_t	len;
	char	first;
	char	last;

	len = strlen(str);
	if (len == 0)
		return ;
	first = str[0];
	last = str[len - 1];
	fully_quoted = 0;
	if ((first == '"' && last == '"') || (first == '\'' && last == '\''))
		fully_quoted = 1;
	if (fully_quoted)
		remove_outer_quotes(str);
	else
		remove_all_quotes(str);
}
