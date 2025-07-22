/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 01:02:14 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/04 01:55:47 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*remove_quotes(char *input, t_shell *shell)
{
	size_t	len;
	char	*output;
	size_t	i;
	size_t	j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	output = malloc(len + 1);
	if (!output)
		ft_clean_exit(input, shell, NULL, NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[i] != '"' && input[i] != '\'')
			output[j++] = input[i];
		i++;
	}
	output[j] = '\0';
	free (input);
	return (output);
}
