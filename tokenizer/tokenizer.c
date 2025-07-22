/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:14:08 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/20 14:57:57 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	quote_handler(const char *str, int i, t_quote_state *state)
{
	if (state->w_start < 0 && (str[i] == '\'' || str[i] == '\"')
		&& state->in_quotes == false)
	{
		state->in_quotes = true;
		state->opening_quote = str[i];
		state->w_start = i;
	}
	else if (state->w_start >= 0 && (str[i] == '\'' || str[i] == '\"')
		&& state->in_quotes == false)
	{
		state->in_quotes = true;
		state->opening_quote = str[i];
	}
	else if (state->w_start >= 0 && state->in_quotes == true
		&& str[i] == state->opening_quote)
		state->in_quotes = false;
}

static void	handle_word_boundaries(int i, t_quote_state *state,
		t_shell *shell, t_input_context *ctx)
{
	char	*tmp;
	int		checker;

	if (ctx->str[i] != '|' && ctx->str[i] != '<'
		&& ctx->str[i] != '>' && state->w_start < 0)
		state->w_start = i;
	else if (((ctx->str[i] == '|' || ctx->str[i] == '<' || ctx->str[i] == '>')
			|| (size_t)i == ft_strlen(ctx->str))
		&& state->w_start >= 0 && state->in_quotes == false)
	{
		tmp = ft_substrword(ctx->str, state->w_start, i, shell);
		if (tmp == NULL)
			ft_clean_exit(NULL, shell, NULL, NULL);
		checker = add_token(shell, tmp, WORD, ctx->rank);
		if (checker == 1)
			ft_clean_exit(tmp, shell, NULL, NULL);
		state->w_start = -1;
		free(tmp);
	}
}

static void	handle_single_operator(int i, t_shell *shell, t_input_context *ctx)
{
	char	*tmp;
	int		checker;

	checker = 0;
	tmp = ft_substrword(ctx->str, i, i + 1, shell);
	if (tmp == NULL)
		ft_clean_exit(NULL, shell, NULL, NULL);
	if (ctx->str[i] == '<')
		checker = add_token(shell, tmp, IN, ctx->rank);
	if (checker == 1)
		ft_clean_exit(tmp, shell, NULL, NULL);
	else if (ctx->str[i] == '>')
		checker = add_token(shell, tmp, OUT, ctx->rank);
	if (checker == 1)
		ft_clean_exit(tmp, shell, NULL, NULL);
	else if (ctx->str[i] == '|')
		checker = add_token(shell, tmp, PIPE, ctx->rank);
	if (checker == 1)
		ft_clean_exit(tmp, shell, NULL, NULL);
	free(tmp);
}

//handles the token making in case of a heredoc or append operator
//bool in order to skip an extra space in the initial string
static bool	handle_double_operator(int i, t_shell *shell, t_input_context *ctx)
{
	char	*tmp;
	int		checker;

	checker = 0;
	if (ctx->str[i + 1] && ((ctx->str[i] == '<' && ctx->str[i + 1] == '<')
			|| (ctx->str[i] == '>' && ctx->str[i + 1] == '>')))
	{
		tmp = ft_substrword(ctx->str, i, i + 2, shell);
		if (tmp == NULL)
			ft_clean_exit(NULL, shell, NULL, NULL);
		if (ctx->str[i] == '<')
			checker = add_token(shell, tmp, HDOC, ctx->rank);
		if (checker == 1)
			ft_clean_exit(tmp, shell, NULL, NULL);
		if (ctx->str[i] == '>')
			checker = add_token(shell, tmp, APPEND, ctx->rank);
		if (checker == 1)
			ft_clean_exit(tmp, shell, NULL, NULL);
		free(tmp);
		return (true);
	}
	return (false);
}

//s stands for shell (struct) and r for rank (split)
//thanks for nothing norminette
void	tokenizer(t_shell *s, int r)
{
	int				i;
	t_quote_state	state;
	t_input_context	ctx;

	i = 0;
	ctx.str = s->splitted[r];
	ctx.rank = r;
	state.w_start = -1;
	state.in_quotes = false;
	while ((size_t)i <= ft_strlen(s->splitted[r]))
	{
		quote_handler(s->splitted[r], i, &state);
		handle_word_boundaries(i, &state, s, &ctx);
		if ((s->splitted[r][i] == '<' || s->splitted[r][i] == '>'
				|| s->splitted[r][i] == '|')
				&& state.in_quotes == false)
		{
			if (handle_double_operator(i, s, &ctx))
				i++;
			else if (s->splitted[r][i] == '<' || s->splitted[r][i] == '>'
					|| s->splitted[r][i] == '|')
				handle_single_operator(i, s, &ctx);
		}
		i++;
	}
}
