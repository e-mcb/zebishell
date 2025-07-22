/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:30:41 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/23 16:40:43 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_substrword(char *str, int start, int end, t_shell *shell)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (end - start + 1));
	if (!word)
		ft_clean_exit(NULL, shell, NULL, NULL);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

char	*trim_quotes(char *str, t_shell *shell)
{
	int		start;
	int		end;
	char	*trimmed;

	start = 0;
	end = ft_strlen(str) - 1;
	if (str[start] == '"' || str[start] == '\'')
		start++;
	if (str[end] == '"' || str[end] == '\'')
		end--;
	trimmed = malloc(end - start + 2);
	if (!trimmed)
		ft_clean_exit(NULL, shell, NULL, NULL);
	ft_strlcpy(trimmed, &str[start], end - start + 2);
	return (trimmed);
}

static int	ft_strsize(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	**ft_strdup_array(char **src, t_shell *shell)
{
	int		size;
	int		i;
	char	**dst;

	size = ft_strsize(src);
	dst = malloc(sizeof(char *) * (size + 1));
	if (!dst)
		ft_clean_exit(NULL, shell, NULL, NULL);
	i = 0;
	while (i < size)
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			while (--i >= 0)
				free(dst[i]);
			free(dst);
			return (NULL);
		}
		i++;
	}
	dst[i] = NULL;
	return (dst);
}

char	*strndup_custom(const char *s, size_t n, t_shell *shell)
{
	char	*res;

	res = malloc(n + 1);
	if (!res)
		ft_clean_exit(NULL, shell, NULL, NULL);
	ft_strncpy(res, s, n);
	res[n] = '\0';
	return (res);
}
