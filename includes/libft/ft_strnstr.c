/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 23:48:53 by mzutter           #+#    #+#             */
/*   Updated: 2024/11/01 23:30:59 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if ((big == (NULL) && len <= 0) || (little == (NULL) && len <= 0))
		return (NULL);
	i = 0;
	j = 0;
	if (little[0] == 0)
		return ((char *) big);
	while (big[i] != 0 && i < len)
	{
		while (big[i + j] == little[j] && big[i + j] != 0 && (i + j) < len)
		{
			j++;
			if (little[j] == 0)
				return ((char *) big + i);
		}
		i++;
		j = 0;
	}
	return (0);
}
