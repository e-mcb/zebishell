/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_to_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:39:57 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/25 22:21:50 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*skip_to_pipe(t_token *token)
{
	char	*line;

	while (token != NULL && token->type != PIPE)
	{
		if (token->type == HDOC)
		{
			while (1)
			{
				write(1, ">", 1);
				line = get_next_line(0);
				if ((token->value[0] == 0 && line[0] == '\n') || !line
					|| !ft_strcmp(ft_strtrim(line, "\n"), token->value))
				{
					free(line);
					token = token->next;
					break ;
				}
				free(line);
			}
		}
		token = token->next;
	}
	return (token);
}

char	**duplicate_array_with_extra_space(char **array, t_shell *shell)
{
	int		i;
	int		j ;
	char	**new_array;

	i = 0;
	j = 0;
	while (array && array[i])
		i++;
	new_array = malloc((i + 2) * sizeof(char *));
	if (!new_array)
		ft_clean_exit(NULL, shell, NULL, new_array);
	while (j < i)
	{
		new_array[j] = ft_strdup(array[j]);
		free(array[j]);
		j++;
	}
	new_array[i] = NULL;
	new_array[i + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}

char	**add_string_to_array(char **array, char *str, t_shell *shell)
{
	char	**new_array;
	int		i;

	i = 0;
	new_array = duplicate_array_with_extra_space(array, shell);
	while (new_array[i])
		i++;
	if (str)
	{
		new_array[i] = ft_strdup(str);
		if (!new_array[i])
			ft_clean_exit(NULL, shell, NULL, new_array);
	}
	else
		new_array[i] = NULL;
	return (new_array);
}

void	create_exec(t_shell *shell)
{
	t_exec	*exec;
	t_token	*tmp;
	t_exec	*last;

	exec = new_node(NULL);
	last = exec;
	tmp = shell->token;
	while (tmp)
	{
		if (is_redir(tmp) || tmp->type == HDOC)
		{
			tmp = handle_redir(last, tmp, shell);
			if (!tmp)
				break ;
		}
		if (tmp->type == ARG || tmp->type == CMD)
			last->arr = add_string_to_array(last->arr, tmp->value, shell);
		if (tmp->type == PIPE)
		{
			last->next = new_node(exec);
			last = last->next;
		}
		tmp = tmp->next;
	}
	shell->exec = exec;
}
