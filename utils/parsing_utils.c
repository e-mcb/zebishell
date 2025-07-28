/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 21:55:23 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/26 00:25:16 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

int	ft_has_invalid_quotes(const char *str)
{
	int		i;
	bool	in_quotes;
	char	opening_quote;

	i = 0;
	in_quotes = false;
	opening_quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			if (!in_quotes)
			{
				in_quotes = true;
				opening_quote = str[i];
			}
			else if (str[i] == opening_quote)
				in_quotes = false;
		}
		i++;
	}
	return (in_quotes);
}

int	is_str_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

void	ft_parsing(char *input, t_shell *shell)
{
	int		i;
	t_token	*tmp;
	t_token	*t;

	i = -1;
	shell->splitted = ft_split2(input);
	if (shell->splitted == NULL)
		ft_clean_exit(input, shell, NULL, NULL);
	free (input);
	while (shell->splitted[++i])
		tokenizer(shell, i);
	t = shell->token;
	while (t)
	{
		if (((t->value[0] == '"' && t->value[1] == '"')
				|| (t->value[0] == '\'' && t->value[1] == '\''))
			&& t->value[2] == 0)
		{
			free(t->value);
			t->value = ft_strdup("");
		}
		t = t->next;
	}
	if (shell->splitted != NULL)
		ft_free_str_array(shell->splitted);
	refine_token_type(shell->token);
	tmp = shell->token;
	while (tmp)
	{
		if (ft_strchr(tmp->value, '\'') || ft_strchr(tmp->value, '"'))
			tmp->in_quotes_hdoc = true;
		tmp = tmp->next;
	}
	expand(shell);
	second_refine_token_type(shell->token, shell);
	shell->splitted = NULL;
}
