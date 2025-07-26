/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   countsegments.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:35:14 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/26 02:07:50 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_case_only_dollar(char *input, t_expand *ex)
{
	if (input[ex->i + 1] == ' ' || input[ex->i + 1] == '\0'
		|| (input[ex->i + 1] == '\"' && ex->in_double_quote)
		|| input[ex->i + 1] == '+' || input[ex->i + 1] == '='
		|| input[ex->i + 1] == ':')
		return (1);
	return (0);
}

static void	process_single_quote(t_expand *ex, char *input)
{
	if (ex->i > ex->start)
		ex->count++;
	if (input[ex->i + 1] == '"' && !ex->in_single_quote)
	{
		ex->i += 2;
		ex->count++;
		ex->start = ex->i;
	}
	ex->in_single_quote = !ex->in_single_quote;
	ex->i++;
	ex->start = ex->i;
}

static void	process_double_quote(t_expand *ex, char *input)
{
	if (ex->i > ex->start)
		ex->count++;
	if (input[ex->i + 1] == '"' && !ex->in_double_quote)
	{
		ex->i += 2;
		ex->count++;
		ex->start = ex->i;
	}
	else
	{
		ex->in_double_quote = !ex->in_double_quote;
		ex->i++;
		ex->start = ex->i;
	}
}

static void	process_dollar(char *input, t_expand *ex)
{
	if (ex->i > ex->start)
		ex->count++;
	if (input[ex->i + 1] && (ft_isdigit(input[ex->i + 1])
			|| input[ex->i + 1] == '$'))
		ex->i += 2;
	else if (is_case_only_dollar(input, ex))
	{
		ex->count++;
		ex->i += 1;
	}
	else if (input[ex->i + 1] == '?')
	{
		ex->count++;
		ex->i += 2;
	}
	else
	{
		ex->i++;
		while (input[ex->i]
			&& (ft_isalnum(input[ex->i]) || input[ex->i] == '_'))
			ex->i++;
		ex->count++;
	}
	ex->start = ex->i;
}

//Determine the number of segments needed before performing 'splitAndExpand'
int	ft_count_segments(char *input)
{
	t_expand	ex;

	if (!input)
		return (0);
	ex.i = 0;
	ex.j = 0;
	ex.count = 0;
	ex.start = 0;
	ex.in_single_quote = 0;
	ex.in_double_quote = 0;
	ex.result = NULL;
	while (input[ex.i])
	{
		if (input[ex.i] == '\'' && !ex.in_double_quote)
			process_single_quote(&ex, input);
		else if (input[ex.i] == '"' && !ex.in_single_quote)
			process_double_quote(&ex, input);
		else if (input[ex.i] == '$' && !ex.in_single_quote)
			process_dollar(input, &ex);
		else
			ex.i++;
	}
	if (ex.i > ex.start)
		ex.count++;
	return (ex.count);
}
