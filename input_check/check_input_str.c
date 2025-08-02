/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 23:19:50 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/06 20:54:34 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_operator(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

static size_t	strlen_no_trailing_ws(const char *str)
{
	size_t	len;
	size_t	last_non_ws_index;

	len = 0;
	last_non_ws_index = 0;
	while (str[len] != '\0')
	{
		if (!isspace((unsigned char)str[len]))
			last_non_ws_index = len + 1;
		len++;
	}
	return (last_non_ws_index);
}

int	string_error(char *input)
{
	size_t	trimmed_len;

	trimmed_len = strlen_no_trailing_ws(input);
	if (trimmed_len == 0)
		return (0);
	if (input[0] == '|' || input[trimmed_len - 1] == '|')
	{
		ft_putstr_fd(PIPE_FIRST_LAST, 2);
		return (1);
	}
	if (is_operator(input[trimmed_len - 1]))
	{
		ft_putstr_fd(OPERATOR_EXTREMITY, 2);
		return (1);
	}
	return (0);
}
