/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 23:45:47 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/25 22:39:31 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	*free_tabs(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free (arr);
	return (NULL);
}

static int	count_words(char const *s, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (s[i] == c && s[i] != 0)
		i++;
	while (s[i] != 0)
	{
		if (s[i] != c && s[i] != 0)
			k = 1;
		if (s[i] == c)
		{
			while (s[i] == c && s[i] != 0)
				i++;
			if (s[i] != 0)
				j++;
		}
		else
			i++;
	}
	return (j + k);
}

static char	*fill_tab(const char *str, int start, int end)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = malloc((end - start + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	while (start < end)
		new_str[i++] = str[start++];
	new_str[i] = '\0';
	return (new_str);
}

static void	init_vars(size_t *i, size_t *j, int *k)
{
	*i = 0;
	*j = 0;
	*k = -1;
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		k;
	char	**str;

	if (!s)
		return (NULL);
	str = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	init_vars(&i, &j, &k);
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && k < 0)
			k = i;
		else if ((s[i] == c || i == ft_strlen(s)) && k >= 0)
		{
			str[j++] = fill_tab(s, k, i);
			if (str[j - 1] == NULL)
				return (free_tabs(str));
			k = -1;
		}
		i++;
	}
	return (str[j] = 0, str);
}
