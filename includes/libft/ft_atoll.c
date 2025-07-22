/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:57:27 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/15 13:46:58 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoll(const char *nptr)
{
	long long	i;
	long long	r;
	long long	s;

	i = 0;
	r = 0;
	s = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == 45)
	{
		s = -1;
		i++;
	}
	else if (nptr[i] == 43)
	{
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		r = (r * 10) + (nptr[i] - 48);
		i++;
	}
	return (r * s);
}
