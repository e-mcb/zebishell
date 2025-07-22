/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bool_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:29:53 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/15 11:12:35 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_quote(char c)
{
	return (c == '"' || c == '\'' || c == '{');
}

bool	is_closing_quote(char c, char opening_quote)
{
	if (opening_quote == '"' && c == '"')
		return (true);
	if (opening_quote == '\'' && c == '\'')
		return (true);
	if (opening_quote == '{' && c == '}')
		return (true);
	return (false);
}

bool	handle_quotes(char c, bool *in_quotes, char *opening_quote)
{
	if (is_quote(c) && !(*in_quotes))
	{
		*in_quotes = true;
		*opening_quote = c;
		return (true);
	}
	else if (is_closing_quote(c, *opening_quote) && *in_quotes)
	{
		*in_quotes = false;
		*opening_quote = '\0';
		return (true);
	}
	return (false);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}
