/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitandexpand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:20:51 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/22 00:16:46 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_ex(t_expand *ex)
{
	ex->i = 0;
	ex->j = 0;
	ex->count = 0;
	ex->start = 0;
	ex->in_single_quote = false;
	ex->in_double_quote = false;
}

static void	process_single_quote(const char *input,
	t_expand *ex, t_shell *shell)
{
	if (ex->i > ex->start)
		ex->result[ex->j++] = strndup_custom(input + ex->start,
				ex->i - ex->start, shell);
	if (ex->in_single_quote == true)
		ex->in_single_quote = false;
	else if (ex->in_single_quote == false)
		ex->in_single_quote = true;
	ex->i++;
	ex->start = ex->i;
}

static void	process_double_quote(const char *input,
	t_expand *ex, t_shell *shell)
{
	if (ex->i > ex->start)
		ex->result[ex->j++] = strndup_custom(input + ex->start,
				ex->i - ex->start, shell);
	ex->in_double_quote = !ex->in_double_quote;
	ex->i++;
	ex->start = ex->i;
}

static void	process_dollar(char *input, t_expand *ex, t_shell *shell)
{
	if (ex->i > ex->start)
	{
		ex->result[ex->j++] = strndup_custom(input + ex->start,
				ex->i - ex->start, shell);
		ex->start = ex->i;
	}
	if (input[ex->i + 1] == ' '
		|| input[ex->i + 1] == '\0')
		case_only_dollar(ex);
	else if (input[ex->i + 1] == '?')
		case_question_mark(ex, shell);
	else if (ft_isdigit(input[ex->i + 1]) || input[ex->i + 1] == '$')
	{
		ex->i += 2;
		ex->start = ex->i;
	}
	else
		case_env_var(ex, input, shell);
}

char	**split_and_expand(char *input, t_shell *shell)
{
	t_expand	ex;

	init_ex(&ex);
	ex.result = malloc(sizeof(char *) * (ft_count_segments(input) + 1));
	if (!ex.result)
		ft_clean_exit(NULL, shell, NULL, NULL);
	while (input[ex.i])
	{
		if (input[ex.i] == '\'' && !ex.in_double_quote)
			process_single_quote(input, &ex, shell);
		else if (input[ex.i] == '"' && !ex.in_single_quote)
			process_double_quote(input, &ex, shell);
		else if (input[ex.i] == '$' && !ex.in_single_quote)
			process_dollar(input, &ex, shell);
		else
			ex.i++;
	}
	if (ex.i > ex.start)
		ex.result[ex.j++] = strndup_custom(input + ex.start,
				ex.i - ex.start, shell);
	ex.result[ex.j] = NULL;
	return (ex.result);
}
