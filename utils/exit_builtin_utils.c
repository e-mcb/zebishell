/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:02:26 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/20 17:02:27 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_check_limits(unsigned long long result, int digit, int sign)
{
	if (result > (unsigned long long)(LLONG_MAX / 10))
		return (1);
	if (result == (unsigned long long)(LLONG_MAX / 10))
	{
		if (sign == 1 && digit > (LLONG_MAX % 10))
			return (1);
		if (sign == -1 && digit > -(LLONG_MIN % 10))
			return (1);
	}
	return (0);
}

static int	ft_parse_number(const char *nptr,
							unsigned long long *result, int *sign)
{
	int	i;
	int	digit;

	i = 0;
	*sign = 1;
	*result = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			*sign = -1;
		i++;
	}
	if (nptr[i] < '0' || nptr[i] > '9')
		return (1);
	while (ft_isdigit(nptr[i]))
	{
		digit = nptr[i] - '0';
		if (ft_check_limits(*result, digit, *sign))
			return (1);
		*result = (*result * 10) + digit;
		i++;
	}
	return (i);
}

int	is_out_of_range(const char *nptr)
{
	unsigned long long	result;
	int					sign;
	int					parsed_len;

	parsed_len = ft_parse_number(nptr, &result, &sign);
	if (nptr[parsed_len] != '\0')
		return (1);
	return (0);
}
