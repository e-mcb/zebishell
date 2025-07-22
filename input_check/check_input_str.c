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

int	string_error(char *input)
{
	if (input[0] == '|' || input[ft_strlen(input) - 1] == '|')
	{
		ft_putstr_fd(PIPE_FIRST_LAST, 2);
		return (1);
	}
	if (is_operator(input[ft_strlen(input) - 1]))
	{
		ft_putstr_fd(OPERATOR_EXTREMITY, 2);
		return (1);
	}
	return (0);
}
