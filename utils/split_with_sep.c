/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_sep.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:49:00 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/06 21:32:37 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	next_chunk_end(const char *s,
	size_t start, bool (*is_sep)(char))
{
	size_t	i;

	i = start;
	while (s[i] && is_sep(s[i]))
		i++;
	while (s[i] && !is_sep(s[i]))
		i++;
	while (s[i] && is_sep(s[i]))
		i++;
	return (i);
}

char	**chunk_handl(char **res, size_t *count_ptr,
	size_t *capacity_ptr, char *chunk)
{
	char	**new_res;
	size_t	count;
	size_t	capacity;

	count = *count_ptr;
	capacity = *capacity_ptr;
	if (count + 1 >= capacity)
	{
		capacity = capacity * 2;
		new_res = malloc(sizeof(char *) * capacity);
		if (!new_res)
			return (NULL);
		ft_memcpy(new_res, res, sizeof(char *) * count);
		free(res);
		res = new_res;
		*capacity_ptr = capacity;
	}
	res[count] = chunk;
	*count_ptr = count + 1;
	return (res);
}

char	**split_keep_separators(const char *s,
	bool (*is_sep)(char), t_shell *shell)
{
	char		**res;
	t_splitter	splitter;
	char		*chunk;

	splitter.i = 0;
	splitter.start = 0;
	splitter.count = 0;
	splitter.capacity = 4;
	res = malloc(sizeof(char *) * splitter.capacity);
	if (!res)
		ft_clean_exit(NULL, shell, NULL, NULL);
	while (s[splitter.i])
	{
		splitter.i = next_chunk_end(s, splitter.start, is_sep);
		chunk = ft_substr(s, splitter.start, splitter.i - splitter.start);
		if (!chunk)
			return (NULL);
		res = chunk_handl(res, &(splitter.count), &(splitter.capacity), chunk);
		if (!res)
			return (NULL);
		splitter.start = splitter.i;
	}
	res[(splitter.count)] = NULL;
	return (res);
}
