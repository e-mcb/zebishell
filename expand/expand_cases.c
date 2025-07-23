/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:29:55 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/21 22:49:16 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

static char	*get_env_value(char *name, t_shell *shell)
{
	char	*value;

	value = ft_getenv(name, shell);
	if (!value)
		return (NULL);
	return (ft_strdup(value));
}

void	case_only_dollar(t_expand *ex)
{
	ex->result[ex->j++] = ft_strdup("$");
	ex->i++;
	ex->start = ex->i;
}

void	case_question_mark(t_expand *ex, t_shell *shell)
{
	char	*value;
	char	*alpha;

	alpha = ft_itoa(shell->exit_status);
	value = ft_strdup(alpha);
	if (!value)
		ft_clean_exit(NULL, shell, NULL, NULL);
	free(alpha);
	ex->result[ex->j++] = value;
	ex->i += 2;
	ex->start = ex->i;
}

void	case_env_var(t_expand *ex, char *input, t_shell *shell)
{
	char	*varname;
	char	*value;

	ex->i++;
	ex->start = ex->i;
	while (input[ex->i] && !ft_isspace(input[ex->i])
		&& input[ex->i] != '"' && input[ex->i] != '\'' && input[ex->i] != '$'
		&& (ft_isalnum(input[ex->i]) || input[ex->i] == '_'))
		ex->i++;
	varname = strndup_custom(input + ex->start, ex->i - ex->start, shell);
	value = get_env_value(varname, shell);
	if (value)
		ex->result[ex->j++] = value;
	free(varname);
	ex->start = ex->i;
}
