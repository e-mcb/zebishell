/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_refine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:12:13 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 22:04:48 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*token_creator(char *str, t_shell *shell)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		ft_clean_exit(NULL, shell, NULL, NULL);
	if (str)
		new_token->value = ft_strdup(str);
	else
		new_token->value = NULL;
	new_token->type = -1;
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	new_token->in_quotes = false;
	return (new_token);
}

void	split_cmd(t_token *token, t_shell *shell)
{
	t_token	*new2;
	char	*arr[2];
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!token || token->type != CMD || !token->value)
		return ;
	while (ft_isalpha(token->value[i]))
	{
		i++;
		j++;
	}
	arr[0] = strndup_custom(token->value, j, shell);
	while (is_whitespace(token->value[i]))
		i++;
	if (token->value[i])
		arr[1] = strndup_custom(token->value + i,
				ft_strlen(token->value + i), shell);
	else
		arr[1] = NULL;
	if (!arr[0])
		ft_clean_exit(NULL, shell, NULL, NULL);
	free(token->value);
	token->value = ft_strdup(arr[0]);
	token->type = CMD;
	if (arr[1])
	{
		new2 = token_creator(arr[1], shell);
		if (!new2)
			ft_clean_exit(NULL, shell, NULL, NULL);
		new2->type = ARG;
		new2->next = token->next;
		token->next = new2;
	}
	free(arr[0]);
	if (arr[1])
		free(arr[1]);
}

void	second_refine_token_type(t_token *token, t_shell *shell)
{
	t_token	*t;
	bool	has_cmd;

	has_cmd = false;
	t = token;
	while (t)
	{
		if (t->type == PIPE)
		{
			has_cmd = false;
			t = t->next;
		}
		if (t->type == WORD && !has_cmd)
		{
			t->type = CMD;
			split_cmd(t, shell);
			has_cmd = true;
		}
		if (t->type == WORD && has_cmd)
			t->type = ARG;
		t = t->next;
	}
}
